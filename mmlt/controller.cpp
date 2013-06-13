#include <QDebug>
#include <QFileInfo>

#include <CGAL/bounding_box.h>

#include "sat/sat_problem.h"

#include "utils/logger.h"

#include "controller.h"

const int Controller::SVGPainter::SVG_PADDING = 10;
const double Controller::SVGPainter::SVG_SCALE = 4.0;

Controller::Controller(QCoreApplication& application, const QSettings& settings) :
    timer_(true),
    cplex_solver_(),
    intersection_algorithm_(),
    sat_solution_(),
    stats_(),

    application_(application),
    input_file_(application.arguments().at(1)),
    settings_(settings),

    abort_timer_(settings_.value("controller/abort_timeout", 60*60*1000).toUInt()),
    file_prefix_(
        input_file_.fileName().left(input_file_.fileName().lastIndexOf('.'))
        ),
    points_(input_file_),

    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    segments_(points_),
    triangulation_(points_),

    intersection_graph_(segments_.size())
{
    logger.time("controller initialization", timer_.elapsed());

    logger.print(mmlt_msg("points=%1 segments=%2 convex hull=%3")
                 .arg(points_.size())
                 .arg(segments_.size())
                 .arg(convex_hull_.size()));

    logger.info(
        mmlt_msg("shortest input segment: %1 (%2)")
        .arg(segments_[0].to_string())
        .arg(segment_length_to_string(segments_[0]))
    );

    abort_timer_.start();
    intersection_algorithm_.run(intersection_graph_, segments_);
    abort_timer_.stop();

    if(settings_.value("draw/segments").toBool())
    {
        logger.info(mmlt_msg("Drawing segments..."));

        SVGPainter painter(this, "segments.svg");
        draw_segments(painter);
        draw_points(painter);
    }

    if(settings_.value("draw/intersections").toBool())
    {
        draw_intersections();
    }

    if(settings_.value("draw/separators").toBool())
    {
        draw_separators();
    }
}

void Controller::iteration()
{
    abort_timer_.start();

    ElapsedTimer iteration_timer(true);
    ++stats_.iteration;

    SATProblem sat_problem(intersection_graph_, segments_);

    SegmentIndex new_bound = (stats_.lower_bound() + stats_.upper_bound() + 1)/2;

    logger.info(mmlt_msg("Trying new lower bound: %1").arg(new_bound));

    sat_problem.set_short_segment_range(new_bound, stats_.upper_bound());

    cplex_solver_.solve_decision_problem(
                settings_,
                file_prefix_,
                sat_problem,
                sat_solution_
                );

    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_timer.elapsed());

    // there is no feasible triangulation
    if(sat_solution_.empty())
    {
        if(new_bound > stats_.lower_bound())
        {
            new_bound = new_bound - 1;
        }

        logger.info(mmlt_msg("New upper bound: %1").arg(new_bound));
        stats_.add_upper_bound(new_bound);
    }
    // there is a feasible triangulation
    else
    {
        SegmentIndex sat_bound = sat_solution_.shortest_segment();

        if(sat_bound > stats_.upper_bound())
        {
            sat_bound = stats_.upper_bound();
        }

        logger.info(mmlt_msg("New lower bound: %1").arg(sat_bound));
        stats_.add_lower_bound(sat_bound);

        if(settings_.value("draw/sat_solution").toBool())
        {
            draw_sat_solution();
        }
    }

    if((stats_.gap() < 1) || (stats_.iteration > settings_.value("controller/max_iterations", 20).toUInt()))
    {
        done();
        return;
    }
    else
    {
        QTimer::singleShot(0, this, SLOT(iteration()));
    }

    output_status();

    abort_timer_.stop();
}

void Controller::start()
{
    if(points_.empty())
    {
        logger.error(mmlt_msg("There are no points!"));
        done();
        return;
    }

    pre_solving();

    if((stats_.gap() < 1) || (settings_.value("controller/max_iterations").toUInt() == 0))
    {
        done();
        return;
    }

    /*
    SATProblem sat_problem(intersection_graph_, segments_);
    sat_problem.set_short_segment_range(stats_.lower_bound(), stats_.upper_bound());

    cplex_solver_.solve_optimization_problem(
                settings_,
                file_prefix_,
                sat_problem,
                sat_solution_
                );

    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_timer.elapsed());

    if(sat_solution_.empty())
    {
        logger.error(mmlt_msg("No feasible solution could be found!"));
        done();
        return;
    }

    if(settings_.value("draw/sat_solution").toBool())
    {
        draw_sat_solution();
    }

    SegmentIndex sat_bound = sat_solution_.shortest_segment();
    logger.info(mmlt_msg("shortest SAT solution segment: %1 (%2)")
                .arg(segments_[sat_bound].to_string())
                .arg(segment_length_to_string(segments_[sat_bound])));

    // check bounds
    MMLT_postcondition(sat_problem.lower_bound() <= sat_bound);
    if(sat_bound > sat_problem.upper_bound())
    {
        sat_bound = sat_problem.upper_bound();
    }

    stats_.add_lower_bound(sat_bound);
    stats_.add_upper_bound(sat_bound);

    if(stats_.gap() < 1)
    {
        done();
        return;
    }
    */

    output_status();

    // next iteration
    QTimer::singleShot(0, this, SLOT(iteration()));
}

void Controller::done()
{
    if(!sat_solution_.empty())
    {
        logger.info(mmlt_msg("Adding separators and non-separable segments to triangulation..."));
        for(const SegmentIndex& seg_index : sat_solution_)
        {
            logger.debug(mmlt_msg("Segment index: %1").arg(seg_index));
            const Segment& segment = segments_[seg_index];
            triangulation_.insert_constraint(segment.source(), segment.target());
        }

        const SegmentIndex& shortest_triangulation_segment =
                triangulation_.shortest_segment(segments_);

        logger.info(
            mmlt_msg("shortest constrained triangulation segment: %1 (%2)")
            .arg(segments_[shortest_triangulation_segment].to_string())
            .arg(segment_length_to_string(segments_[shortest_triangulation_segment]))
        );

        if(settings_.value("draw/triangulation").toBool())
        {
            draw_triangulation();
        }
    }

    logger.time("total", timer_.elapsed());

    output_status();

    logger.print(mmlt_msg("iterations=%1 gap=%2").arg(stats_.iteration).arg(stats_.gap()));
    logger.info(mmlt_msg("Done."));
    application_.quit();
}

void Controller::draw_bounds() const
{
    logger.info(mmlt_msg("Drawing bounds..."));

    SVGPainter painter(this, QString("bounds_%1.svg").arg(stats_.iteration));
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
    logger.info(mmlt_msg("Drawing intersections..."));

    SegmentIndex segment_index = 0;

    for(const Intersections& intersections : intersection_graph_)
    {
        SVGPainter painter(this, QString("%1_intersections.svg").arg(segment_index, 5, 10, QChar('0')));

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
    logger.info(mmlt_msg("Drawing SAT solution..."));

    SVGPainter painter(this, QString("sat_%1.svg").arg(stats_.iteration));

    //draw_segments(painter);

    painter.setPenColor(QColor(255, 255, 0));
    segments_.draw_range(painter, stats_.lower_bound(), stats_.upper_bound());

    painter.setPenColor(QColor(255, 0, 0));
    sat_solution_.draw_short_segments(painter, stats_.upper_bound(), segments_);

    painter.setPenColor(QColor(0, 255, 0));
    sat_solution_.draw_separators(painter, stats_.upper_bound(), segments_);

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
        SVGPainter painter(this, QString("separators_%1.svg").arg(index));

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
    logger.info(mmlt_msg("Drawing triangulation..."));

    SVGPainter painter(this, "solution.svg");

    //draw_segments(painter);

    painter.setPenColor(QColor(255, 0, 0));
    triangulation_.draw(painter);

    draw_points(painter);
}

void Controller::output_status() const
{
    logger.stats(stats_);

    if(settings_.value("draw/bounds").toBool())
    {
        draw_bounds();
    }
}

void Controller::pre_solving()
{
    ElapsedTimer presolving_timer(true);

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

    logger.time("pre solving", presolving_timer.elapsed());
}
