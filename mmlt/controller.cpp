#include <QDebug>
#include <QFileInfo>
#include <QTimer>

#include <CGAL/bounding_box.h>

#include "cplex/sat_problem.h"
#include "utils/logger.h"

#include "controller.h"

const double Controller::SVGPainter::SVG_SCALE = 4.0;

Controller::Controller(QCoreApplication& application, const QSettings& settings) :
    start_time_(QTime::currentTime()),
    application_(application),
    input_file_(application.arguments().at(1)),
    file_prefix_(QFileInfo(input_file_).completeBaseName()),
    points_(input_file_),
    settings_(settings),
    stats_(),
    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    segments_(points_),
    intersection_graph_(points_, segments_),
    sat_problem_(points_.size(), segments_.size(), convex_hull_.size(), intersection_graph_.intersection_groups())
{
    logger.print(mmlt_msg("points: %1").arg(points_.size()));
    logger.print(mmlt_msg("segments: %1").arg(segments_.size()));
    logger.print(mmlt_msg("convex hull: %1").arg(convex_hull_.size()));

    if(settings_.value("draw/intersection_graph").toBool())
    {
        logger.info(mmlt_msg("Drawing intersection graph..."));
        SVGPainter painter(this, "_igraph.svg");
        painter.setPenColor(QColor(120, 120, 120));
        intersection_graph_.draw_igraph(painter);
    }

    if(settings_.value("draw/intersection_groups").toBool())
    {
        draw_igroups();
    }

    logger.time("controller initialization", start_time_.msecsTo(QTime::currentTime()));
}

void Controller::iteration()
{
    ++stats_.iteration;

    sat_problem_.solve(sat_solution_, (stats_.lower_bound() + stats_.upper_bound())/2 + 1);

    if(sat_solution_.empty())
    {
        stats_.add_upper_bound((stats_.lower_bound() + stats_.upper_bound())/2);
    }
    else
    {
        stats_.add_lower_bound(sat_solution_.shortest_segment());
        if(settings_.value("draw/sat_solution").toBool())
        {
            draw_sat_solution();
        }
    }

    if((stats_.gap() < 1) || (stats_.iteration > 20))
    {
        application_.quit();
    }
    else
    {
        QTimer::singleShot(0, this, SLOT(iteration()));
    }

    logger.stats(stats_);
    if(settings_.value("draw/bounds").toBool())
    {
        draw_bounds();
    }
}

void Controller::start()
{
    if(points_.empty())
    {
        logger.error(mmlt_msg("There are no points!"));
        application_.quit();
        return;
    }

    pre_solving();

    sat_problem_.solve(sat_solution_, stats_.lower_bound());

    if(sat_solution_.empty())
    {
        logger.error(mmlt_msg("No feasible solution could be found!"));
        application_.quit();
        return;
    }

    stats_.add_lower_bound(sat_solution_.shortest_segment());
    if(settings_.value("draw/sat_solution").toBool())
    {
        draw_sat_solution();
    }

    if(stats_.gap() < 1)
    {
        application_.quit();
    }
    else
    {
        // next iteration
        QTimer::singleShot(0, this, SLOT(iteration()));
    }

    logger.stats(stats_);
    if(settings_.value("draw/bounds").toBool())
    {
        draw_bounds();
    }
}

void Controller::draw_bounds()
{
    logger.info(mmlt_msg("Drawing bounds..."));

    SVGPainter painter(this, QString("_bounds_%1.svg").arg(stats_.iteration));
    painter.setPenColor(QColor(120, 120, 120));
    intersection_graph_.draw_igraph(painter);

    painter.setPenColor(QColor(0, 255, 0));
    segments_[stats_.lower_bound()].draw(painter);
    painter.setPenColor(QColor(255, 0, 0));
    segments_[stats_.upper_bound()].draw(painter);
}

void Controller::draw_igroups()
{
    logger.info(mmlt_msg("Drawing intersection groups..."));

    IntersectionGroupIndex igroup_index = 0;

    foreach(const IntersectionGroup& igroup, intersection_graph_.intersection_groups())
    {
        SVGPainter painter(this, QString("_%1_igroup.svg").arg(igroup_index, 5, 10));
        painter.setPenColor(QColor(120, 120, 120));
        intersection_graph_.draw_igraph(painter);

        painter.setPenColor(QColor(255, 0, 0));
        intersection_graph_.draw_igroup(painter, igroup);

        ++igroup_index;
    }
}

void Controller::draw_sat_solution()
{
    logger.info(mmlt_msg("Drawing SAT solution..."));

    SVGPainter painter(this, QString("_sat_%1.svg").arg(stats_.iteration));
    painter.setPenColor(QColor(120, 120, 120));
    intersection_graph_.draw_igraph(painter);
    painter.setPenColor(QColor(255, 0, 0));
    sat_solution_.draw(painter, segments_);
}

void Controller::pre_solving()
{
    // shortest segment of convex hull must be part of triangulation
    stats_.add_upper_bound(convex_hull_.shortest_segment(segments_));

    // each non-intersected segment must be part of the triangulation
    stats_.add_upper_bound(intersection_graph_.shortest_nonintersecting_segment());
}
