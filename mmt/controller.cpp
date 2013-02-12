#include <QDebug>
#include <QFileInfo>
#include <QTimer>

#include <CGAL/bounding_box.h>

#include "cplex/sat_problem.h"
#include "utils/json_parser.h"
#include "utils/logger.h"

#include "controller.h"

const double Controller::SVGPainter::SVG_SCALE = 4.0;

Controller::Controller(QFile &input_file, const Parameters &parameters) :
    file_prefix_(QFileInfo(input_file).completeBaseName()),
    parameters_(parameters),
    points_(JSONParser::parse(input_file)),
    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    segments_(points_),
    intersection_graph_(points_, segments_),
    sat_problem_(points_.size(), segments_.size(), convex_hull_.size(), intersection_graph_.intersection_groups())
{

    logger.print(mmt_msg("points: %1").arg(points_.size()));
    logger.print(mmt_msg("segments: %1").arg(segments_.size()));
    logger.print(mmt_msg("convex hull: %1").arg(convex_hull_.size()));

    if(parameters.draw_igraph)
    {
        logger.info(mmt_msg("Drawing intersection graph..."));
        SVGPainter painter(this, "_igraph.svg");
        painter.setPenColor(QColor(120, 120, 120));
        intersection_graph_.draw_igraph(painter);
    }

    if(parameters.draw_igroups)
    {
        logger.info(mmt_msg("Drawing intersection groups..."));

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
}

void Controller::init()
{
    logger.info(mmt_msg("Initializing controller..."));

    pre_solving();

    sat_problem_.solve(sat_solution_, stats_.lower_bound());

    if(sat_solution_.empty())
    {
        logger.error(mmt_msg("No feasible solution could be found!"));
        emit done();
    }
    else
    {
        stats_.add_lower_bound(sat_solution_.shortest_segment());
        draw_sat_solution();

        if(stats_.gap() < 1)
        {
            emit done();
        }
        else
        {
            QTimer::singleShot(0, this, SLOT(iteration()));
        }
    }

    logger.stats(stats_);
    draw_bounds();
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
        draw_sat_solution();
    }

    if((stats_.gap() < 1) || (stats_.iteration > 20))
    {
        emit done();
    }
    else
    {
        QTimer::singleShot(0, this, SLOT(iteration()));
    }

    logger.stats(stats_);
    draw_bounds();
}

void Controller::draw_bounds()
{
    SVGPainter painter(this, QString("_bounds_%1.svg").arg(stats_.iteration));
    painter.setPenColor(QColor(120, 120, 120));
    intersection_graph_.draw_igraph(painter);

    painter.setPenColor(QColor(0, 255, 0));
    segments_[stats_.lower_bound()].draw(painter);
    painter.setPenColor(QColor(255, 0, 0));
    segments_[stats_.upper_bound()].draw(painter);
}

void Controller::draw_sat_solution()
{
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
    stats_.add_upper_bound(intersection_graph_.shortest_segment());
}
