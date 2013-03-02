#include <QDebug>
#include <QFileInfo>

#include <CGAL/bounding_box.h>

#include "cplex/sat_problem.h"

#include "utils/logger.h"

#include "controller.h"

const double Controller::SVGPainter::SVG_SCALE = 4.0;

Controller::Controller(QCoreApplication& application, const QSettings& settings) :
    timer_(true),
    sat_solution_(),
    stats_(),

    application_(application),
    input_file_(application.arguments().at(1)),
    settings_(settings),

    abort_timer_(settings_.value("abort_timeout", 60*60*1000).toUInt()),
    file_prefix_(QFileInfo(input_file_).completeBaseName()),
    points_(input_file_),

    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    segments_(points_),
    triangulation_(points_),

    intersection_algorithm_(points_, segments_),
    intersection_graph_(segments_)
{
    logger.time("controller initialization", timer_.elapsed());

    logger.print(mmlt_msg("points=%1 segments=%2 convex hull=%3")
                 .arg(points_.size())
                 .arg(segments_.size())
                 .arg(convex_hull_.size()));

    intersection_algorithm_.run(intersection_graph_);

    if(settings_.value("draw/segments").toBool())
    {
        logger.info(mmlt_msg("Drawing segments..."));

        SVGPainter painter(this, "segments.svg");
        draw_segments(painter);
        draw_points(painter);
    }

    if(settings_.value("draw/intersection_groups").toBool())
    {
        draw_igroups();
    }

    if(settings_.value("draw/separators").toBool())
    {
        draw_separators();
    }
}

void Controller::iteration()
{
#if 0
    abort_timer_.start();

    ElapsedTimer iteration_timer(true);
    ++stats_.iteration;

    sat_problem_.solve(settings_, file_prefix_, sat_solution_, (stats_.lower_bound() + stats_.upper_bound())/2 + 1);


    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_timer.elapsed());

    if(sat_solution_.empty())
    {
        // there is no feasible triangulation
        stats_.add_upper_bound((stats_.lower_bound() + stats_.upper_bound())/2);
    }
    else
    {
        // there is a feasible triangulation
        stats_.add_lower_bound(sat_solution_.shortest_segment());

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
#endif
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

    ElapsedTimer iteration_timer(true);

    SATProblem sat_problem_(intersection_graph_,
                            segments_,
                            stats_.lower_bound(),
                            stats_.upper_bound());

    sat_problem_.solve(settings_, file_prefix_, sat_solution_);

    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_timer.elapsed());

    if(sat_solution_.empty())
    {
        logger.error(mmlt_msg("No feasible solution could be found!"));
        done();
        return;
    }

    stats_.add_lower_bound(sat_solution_.shortest_segment());
    stats_.add_upper_bound(sat_solution_.shortest_segment());

    if(settings_.value("draw/sat_solution").toBool())
    {
        draw_sat_solution();
    }

    if(stats_.gap() < 1)
    {
        done();
        return;
    }

    output_status();

    // next iteration
    QTimer::singleShot(0, this, SLOT(iteration()));
}

void Controller::done() const
{
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

    painter.setPenColor(QColor(0, 255, 0));
    segments_[stats_.lower_bound()].draw(painter);
    painter.setPenColor(QColor(255, 0, 0));
    segments_[stats_.upper_bound()].draw(painter);

    draw_points(painter);
}

void Controller::draw_igroups() const
{
    logger.info(mmlt_msg("Drawing intersection groups..."));

    IntersectionGroupIndex igroup_index = 0;

    for(const IntersectionGroup& igroup : intersection_graph_)
    {
        SVGPainter painter(this, QString("%1_igroup.svg").arg(igroup_index, 5, 10));

        draw_segments(painter);
        painter.setPenColor(QColor(255, 0, 0));
        igroup.draw(painter, segments_);
        draw_points(painter);

        ++igroup_index;
    }
}

void Controller::draw_points(SVGPainter& painter) const
{
    painter.setPenColor(QColor(0, 0, 255));
    points_.draw(painter);
}

void Controller::draw_sat_solution() const
{
    logger.info(mmlt_msg("Drawing SAT solution..."));

    SVGPainter painter(this, QString("sat_%1.svg").arg(stats_.iteration));

    draw_segments(painter);
    painter.setPenColor(QColor(255, 0, 0));
    sat_solution_.draw(painter, segments_);
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

    for(SegmentIndex index = 0; index < intersection_algorithm_.shortest_nonintersecting_segment; ++index)
    {
        SVGPainter painter(this, QString("separators_%1.svg").arg(index));
        draw_segments(painter);

        const Segment& segment = segments_[index];

        painter.setPenColor(QColor(255, 0, 0));
        //segments_[intersection_graph_.longest_intersecting_segment(index)].draw(painter);
        for(const IntersectionGroupIndex& igroup : segment.data().intersection_groups)
        {
            for(const SegmentIndex& separator : intersection_graph_[igroup])
            {
                if(separator == index)
                {
                    continue;
                }

                segments_[separator].draw(painter);
            }
        }

        painter.setPenColor(QColor(0, 255, 0));
        segment.draw(painter);

        draw_points(painter);
    }
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

    // shortest segment of convex hull must be part of triangulation
    stats_.add_upper_bound(convex_hull_.shortest_segment(segments_));

    // each non-intersected segment must be part of the triangulation
    stats_.add_upper_bound(intersection_algorithm_.shortest_nonintersecting_segment);

    logger.time("pre solving", presolving_timer.elapsed());
}
