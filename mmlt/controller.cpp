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
    points_(input_file_),
    settings_(settings),
    stats_(),
    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    delaunay_triangulation_(points_),
    segments_(points_),
    intersection_graph_(points_, segments_),
    sat_problem_(points_.size(), segments_.size(), convex_hull_.size(), intersection_graph_)
{
    logger.time("controller initialization", start_time_.msecsTo(QTime::currentTime()));

    logger.print(mmlt_msg("points=%1 segments=%2 convex hull=%3")
                 .arg(points_.size())
                 .arg(segments_.size())
                 .arg(convex_hull_.size()));

    if(settings_.value("draw/intersection_graph").toBool())
    {
        logger.info(mmlt_msg("Drawing intersection graph..."));
        SVGPainter painter(this, "igraph.svg");
        painter.setPenColor(QColor(120, 120, 120));
        intersection_graph_.draw(painter);
    }

    if(settings_.value("draw/intersection_groups").toBool())
    {
        draw_igroups();
    }
}

void Controller::iteration()
{
    QTime iteration_start_time(QTime::currentTime());
    ++stats_.iteration;

    sat_problem_.solve(settings_, sat_solution_, (stats_.lower_bound() + stats_.upper_bound())/2 + 1);

    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_start_time.msecsTo(QTime::currentTime()));

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
        done();
        return;
    }

    pre_solving();

    if(stats_.gap() < 1)
    {
        done();
        return;
    }

    QTime iteration_start_time(QTime::currentTime());

    sat_problem_.solve(settings_, sat_solution_, stats_.lower_bound());

    logger.time(QString("iteration %1").arg(stats_.iteration), iteration_start_time.msecsTo(QTime::currentTime()));

    if(sat_solution_.empty())
    {
        logger.error(mmlt_msg("No feasible solution could be found!"));
        done();
        return;
    }

    stats_.add_lower_bound(sat_solution_.shortest_segment());
    if(settings_.value("draw/sat_solution").toBool())
    {
        draw_sat_solution();
    }

    if(stats_.gap() < 1)
    {
        done();
        return;
    }

    logger.stats(stats_);
    if(settings_.value("draw/bounds").toBool())
    {
        draw_bounds();
    }

    // next iteration
    QTimer::singleShot(0, this, SLOT(iteration()));
}

void Controller::done()
{
    logger.time("total", start_time_.msecsTo(QTime::currentTime()));

    logger.stats(stats_);
    if(settings_.value("draw/bounds").toBool())
    {
        draw_bounds();
    }

    logger.print(mmlt_msg("iterations=%1 gap=%2").arg(stats_.iteration).arg(stats_.gap()));
    logger.info(mmlt_msg("Done."));
    application_.quit();
}

void Controller::draw_bounds()
{
    logger.info(mmlt_msg("Drawing bounds..."));

    SVGPainter painter(this, QString("bounds_%1.svg").arg(stats_.iteration));
    painter.setPenColor(QColor(120, 120, 120));
    intersection_graph_.draw(painter);

    painter.setPenColor(QColor(0, 255, 0));
    segments_[stats_.lower_bound()].draw(painter);
    painter.setPenColor(QColor(255, 0, 0));
    segments_[stats_.upper_bound()].draw(painter);
}

void Controller::draw_igroups()
{
    logger.info(mmlt_msg("Drawing intersection groups..."));

    IntersectionGroupIndex igroup_index = 0;

    for(const IntersectionGroup& igroup : intersection_graph_)
    {
        SVGPainter painter(this, QString("%1_igroup.svg").arg(igroup_index, 5, 10));
        painter.setPenColor(QColor(120, 120, 120));
        intersection_graph_.draw(painter);

        painter.setPenColor(QColor(255, 0, 0));
        igroup.draw(painter, segments_);

        ++igroup_index;
    }
}

void Controller::draw_sat_solution()
{
    logger.info(mmlt_msg("Drawing SAT solution..."));

    SVGPainter painter(this, QString("sat_%1.svg").arg(stats_.iteration));
    painter.setPenColor(QColor(120, 120, 120));
    intersection_graph_.draw(painter);
    painter.setPenColor(QColor(255, 0, 0));
    sat_solution_.draw(painter, segments_);
}

void Controller::pre_solving()
{
    QTime presolving_start_time(QTime::currentTime());

    // make use of Delaunay triangulation
    stats_.add_lower_bound(delaunay_triangulation_.shortest_segment(segments_));

    // shortest segment of convex hull must be part of triangulation
    stats_.add_upper_bound(convex_hull_.shortest_segment(segments_));

    // each non-intersected segment must be part of the triangulation
    stats_.add_upper_bound(intersection_graph_.shortest_nonintersecting_segment());

    logger.time("pre solving", presolving_start_time.msecsTo(QTime::currentTime()));
}
