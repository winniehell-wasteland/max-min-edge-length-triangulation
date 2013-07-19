#include <QElapsedTimer>

#include "sat/sat_problem.h"

#include "utils/logger.h"

#include "controller.h"

namespace MMLT
{

Controller::Controller(const QString& file_prefix,
                       QFile& input_file,
                       const Settings& settings) :
    cplex_solver_(),
    intersection_algorithm_(),
    last_sat_solution_(),
    stats_(),

    file_prefix_( file_prefix ),
    input_file_( input_file ),
    settings_( settings ),

    points_( input_file_ ),

    bounding_box_(points_),
    convex_hull_(points_),
    segments_(points_),
    triangulation_(points_),

    intersection_graph_(segments_.size())
{
    logger.print( mmlt_msg( "points=%1 segments=%2 convex hull=%3" )
                  .arg(points_.size())
                  .arg(segments_.size())
                  .arg(convex_hull_.size()));

    if( !segments_.empty() )
    {
        const Segment& shortest_input_segment = segments_[0];
        logger.info(
                    mmlt_msg("shortest input segment: %1 (%2)")
                    .arg(shortest_input_segment.to_string())
                    .arg(segment_length_to_string(shortest_input_segment))
                    );
    }

    {
        QElapsedTimer timer;
        timer.start();
        intersection_algorithm_.run(settings_, intersection_graph_, segments_);
        logger.time( mmlt_msg( "intersection graph" ), timer.elapsed() );
    }

    if( settings_.draw && settings_.draw_segments )
    {
        logger.debug(mmlt_msg("Drawing segments..."));

        SVGPainter painter(file_prefix_, "segments.svg", bounding_box_);
        draw_segments(painter);
        draw_points(painter);
    }

    if( settings_.draw && settings_.draw_intersections )
    {
        draw_intersections();
    }

    if( settings_.draw && settings_.draw_separators )
    {
        draw_separators();
    }
}

bool Controller::iteration()
{
    QElapsedTimer iteration_time;
    iteration_time.start();

    ++stats_.iteration;

    SATProblem sat_problem(intersection_graph_, segments_);

    SegmentIndex new_bound = ( stats_.lower_bound() + stats_.upper_bound() + 1 ) / 2;

    logger.debug( mmlt_msg( "Trying new lower bound: %1" )
                  .arg( new_bound ) );

    sat_problem.set_short_segment_range( new_bound, stats_.upper_bound() );

    {
        SATSolution sat_solution;

        QElapsedTimer sat_time;
        sat_time.start();

        cplex_solver_.solve_decision_problem(
                    settings_,
                    file_prefix_,
                    sat_problem,
                    sat_solution
                    );

        stats_.sat_solving_time += sat_time.elapsed();

        // there is no feasible triangulation
        if(sat_solution.empty())
        {
            if(new_bound > stats_.lower_bound())
            {
                new_bound = new_bound - 1;
            }

            logger.debug( mmlt_msg( "New upper bound: %1" )
                          .arg(new_bound));
            stats_.add_upper_bound(new_bound);
        }
        // there is a feasible triangulation
        else
        {
            SegmentIndex sat_bound = sat_solution.shortest_segment();

            if(sat_bound > stats_.upper_bound())
            {
                sat_bound = stats_.upper_bound();
            }

            logger.debug( mmlt_msg( "New lower bound: %1" ).arg(sat_bound) );
            stats_.add_lower_bound(sat_bound);

            // remember last solution
            last_sat_solution_.clear();
            std::copy(sat_solution.begin(), sat_solution.end(), std::back_inserter(last_sat_solution_));

            if( settings_.draw_sat_solution )
            {
                draw_sat_solution();
            }
        }
    }

    logger.time( mmlt_msg( "iteration %1" )
                 .arg(stats_.iteration),
                 iteration_time.elapsed());

    if( ( stats_.gap() < 1 )
            || ( stats_.iteration > settings_.max_iterations ) )
    {
        return false;
    }
    else
    {
        output_status();
        return true;
    }
}

bool Controller::start()
{
    if(points_.empty())
    {
        logger.error( mmlt_msg( "There are no points!" ) );
        return false;
    }

    if( settings_.complete_sat )
    {
        stats_.add_upper_bound( segments_.size() );

        SATProblem sat_problem( intersection_graph_, segments_ );
        sat_problem.set_short_segment_range( stats_.lower_bound(), stats_.upper_bound() );
        SATSolution sat_solution;

        QElapsedTimer sat_time;
        sat_time.start();

        cplex_solver_.solve_optimization_problem(
                    settings_,
                    file_prefix_,
                    sat_problem,
                    sat_solution
                    );

        stats_.sat_solving_time += sat_time.elapsed();

        // there is no feasible triangulation
        if( sat_solution.empty() )
        {
            logger.error( mmlt_msg( "No feasible solution!" ) );
        }
        // there is a feasible triangulation
        else
        {
            SegmentIndex sat_bound = sat_solution.shortest_segment();

            if(sat_bound > stats_.upper_bound())
            {
                sat_bound = stats_.upper_bound();
            }

            stats_.add_lower_bound(sat_bound);

            // remember solution
            last_sat_solution_.clear();
            std::copy(sat_solution.begin(), sat_solution.end(), std::back_inserter(last_sat_solution_));

            if( settings_.draw_sat_solution )
            {
                draw_sat_solution();
            }
        }

        return false;
    }
    else
    {
        pre_solving();

        if( ( stats_.gap() < 1 )
                || ( settings_.max_iterations == 0 ) )
        {
            return false;
        }

        output_status();
    }

    return true;
}

void Controller::done()
{
    if( !segments_.empty() )
    {
        if( !last_sat_solution_.empty() )
        {
            logger.debug( mmlt_msg( "Adding separators and non-separable segments to triangulation..." ) );

            for( const SegmentIndex& seg_index : last_sat_solution_ )
            {
                logger.debug(mmlt_msg("Segment index: %1").arg(seg_index));
                const Segment& segment = segments_[seg_index];
                triangulation_.insert_constraint(segment.source(), segment.target());
            }
        }

        const Segment& shortest_triangulation_segment =
                segments_[triangulation_.shortest_segment(segments_)];

        logger.info(
                    mmlt_msg("shortest constrained triangulation segment: %1 (%2)")
                    .arg(shortest_triangulation_segment.to_string())
                    .arg(segment_length_to_string(shortest_triangulation_segment))
                    );

        if( settings_.draw_triangulation )
        {
            draw_triangulation();
        }
    }

    output_status();

    logger.print(mmlt_msg("iterations=%1 gap=%2").arg(stats_.iteration).arg(stats_.gap()));
    logger.time( mmlt_msg( "SAT solving" ), stats_.sat_solving_time);
}

void Controller::draw_bounds() const
{
    logger.info(mmlt_msg("Drawing bounds..."));

    SVGPainter painter(file_prefix_, QString("bounds_%1.svg").arg(stats_.iteration), bounding_box_);
    draw_segments(painter);

    painter.setPenColor(QColor(255, 255, 0));
    segments_.draw_range(painter, stats_.lower_bound(), stats_.upper_bound());

    painter.setPenColor(QColor(0, 255, 0));
    segments_[stats_.lower_bound()].draw(painter);

    painter.setPenColor(QColor(255, 0, 0));
    segments_[stats_.upper_bound()].draw(painter);

    draw_points(painter);
}

void Controller::draw_intersections() const
{
    logger.info( mmlt_msg( "Drawing intersections..." ) );

    SegmentIndex segment_index = 0;

    for(const Intersections& intersections : intersection_graph_)
    {
        SVGPainter painter(file_prefix_, QString("%1_intersections.svg").arg(segment_index, 5, 10, QChar('0')), bounding_box_);

        draw_segments(painter);

        painter.setPenColor(QColor(0, 255, 0));
        segments_[segment_index].draw(painter);

        painter.setPenColor(QColor(255, 0, 0));
        intersections.draw(painter, segments_);

        draw_points(painter);

        ++segment_index;
    }
}

void Controller::draw_points(SVGPainter& painter) const
{
    painter.setPenColor(QColor(0, 0, 255));
    //int oldwidth = painter.pen().width();
    //painter.setPenWidth(10*oldwidth);
    points_.draw(painter);
    //painter.setPenWidth(oldwidth);
}

void Controller::draw_sat_solution() const
{
    logger.debug( mmlt_msg( "Drawing SAT solution..." ) );

    SVGPainter painter(file_prefix_, QString("sat_%1.svg").arg(stats_.iteration), bounding_box_);

    //draw_segments(painter);

    painter.setPenColor(QColor(255, 255, 0));
    segments_.draw_range(painter, stats_.lower_bound(), stats_.upper_bound());

    painter.setPenColor(QColor(255, 0, 0));
    last_sat_solution_.draw_short_segments(painter, stats_.upper_bound(), segments_);

    painter.setPenColor(QColor(0, 255, 0));
    last_sat_solution_.draw_separators(painter, stats_.upper_bound(), segments_);

    draw_points(painter);
}

void Controller::draw_segments(SVGPainter& painter) const
{
    painter.setPenColor(QColor(120, 120, 120));
    segments_.draw(painter);
}

void Controller::draw_separators() const
{
    logger.info(mmlt_msg("Drawing Separators..."));

    for(SegmentIndex index = 0; index < intersection_algorithm_.shortest_noncrossing_segment_; ++index)
    {
        SVGPainter painter(file_prefix_, QString("separators_%1.svg").arg(index), bounding_box_);

        //draw_segments(painter);


        painter.setPenColor(QColor(255, 0, 0));

        const Intersections& intersections = intersection_graph_[index];

        std::vector<SegmentIndex> separators;
        intersections.find_separators(index, segments_, separators);

        for(const SegmentIndex& separator_index : separators)
        {
            const Segment& separator= segments_[separator_index];
            separator.draw(painter);
        }

        painter.setPenColor(QColor(0, 255, 0));

        const Segment& segment = segments_[index];
        segment.draw(painter);

        draw_points(painter);
    }
}

void Controller::draw_triangulation() const
{
    logger.debug( mmlt_msg( "Drawing triangulation..." ) );

    SVGPainter painter(file_prefix_, "solution.svg", bounding_box_);

    //draw_segments(painter);

    painter.setPenColor(QColor(255, 0, 0));
    triangulation_.draw(painter);

    draw_points(painter);
}

void Controller::output_status() const
{
    logger.stats(stats_);

    if( settings_.draw_bounds )
    {
        draw_bounds();
    }
}

void Controller::pre_solving()
{
    // use non-constrained triangulation as a feasible solution
    const SegmentIndex& shortest_triangulation_segment =
            triangulation_.shortest_segment(segments_);

    stats_.add_lower_bound(shortest_triangulation_segment);

    logger.info(
                mmlt_msg("shortest triangulation segment: %1 (%2)")
                .arg(segments_[shortest_triangulation_segment].to_string())
                .arg(segment_length_to_string(segments_[shortest_triangulation_segment]))
                );

    // shortest segment of convex hull must be part of triangulation
    stats_.add_upper_bound(convex_hull_.shortest_segment(segments_));

    // each non-intersected segment must be part of the triangulation
    stats_.add_upper_bound(intersection_algorithm_.shortest_noncrossing_segment_);
}

}
