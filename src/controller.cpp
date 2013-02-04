#include <QDebug>
#include <QFileInfo>
#include <QTimer>

#include <CGAL/bounding_box.h>

#include "cplex/sat_problem.h"
#include "utils/json_parser.h"
#include "utils/logger.h"

#include "controller.h"

Controller::Controller(QFile &input_file, const Parameters &parameters) :
    file_prefix_(QFileInfo(input_file).completeBaseName()),
    parameters_(parameters),
    points_(JSONParser::parse(input_file)),
    svg_generator_(),
    bounding_box_(CGAL::bounding_box(points_.begin(), points_.end())),
    convex_hull_(points_),
    segments_(points_),
    intersection_graph_(points_, segments_),
    sat_problem_(points_.size(), segments_.size(), convex_hull_.size(), intersection_graph_.intersection_groups())
{
    svg_generator_.setSize(QSize(CGAL::to_double(bounding_box_.xmax()) - CGAL::to_double(bounding_box_.xmin()) + 20,
                                 CGAL::to_double(bounding_box_.ymax()) - CGAL::to_double(bounding_box_.ymin()) + 20) * MMT_SVG_SCALE);

    svg_generator_.setViewBox(QRect(CGAL::to_double(MMT_SVG_SCALE*(bounding_box_.xmin()-10)),
                                    CGAL::to_double(MMT_SVG_SCALE*(bounding_box_.ymin()-10)),
                                    svg_generator_.size().width(),
                                    svg_generator_.size().height()));

    logger.print(mmt_msg("points: %1").arg(points_.size()));
    logger.print(mmt_msg("segments: %1").arg(segments_.size()));
    logger.print(mmt_msg("convex hull: %1").arg(convex_hull_.size()));

    if(parameters.draw_igraph)
    {
        logger.info(mmt_msg("Drawing intersection graph..."));
        SVGPainter painter(this, "_igraph.svg");
        intersection_graph_.draw_igraph(painter);
    }

    if(parameters.draw_igroups)
    {
        logger.info(mmt_msg("Drawing intersection groups..."));

        IntersectionGroupIndex igroup_index = 0;

        foreach(const IntersectionGroup& igroup, intersection_graph_.intersection_groups())
        {
            SVGPainter painter(this, QString("_%1_igroup.svg").arg(igroup_index, 5, 10));
            intersection_graph_.draw_igraph(painter);
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

        SVGPainter painter(this, "_sat.svg");
        intersection_graph_.draw_igraph(painter);
        sat_solution_.draw(painter, segments_);

        if(stats_.gap() < 1)
        {
            logger.stats(stats_);
            emit done();
        }
        else
        {
            QTimer::singleShot(0, this, SLOT(iteration()));
        }
    }

}

void Controller::iteration()
{
    logger.stats(stats_);

    {
        SVGPainter painter(this, QString("_bounds_%1.svg").arg(stats_.iteration));
        intersection_graph_.draw_igraph(painter);

        painter.setPen(QPen(QColor(0, 255, 0), 1.0/MMT_SVG_SCALE));
        segments_[stats_.lower_bound()].draw(painter);
        painter.setPen(QPen(QColor(255, 0, 0), 1.0/MMT_SVG_SCALE));
        segments_[stats_.upper_bound()].draw(painter);
    }

    sat_problem_.solve(sat_solution_, (stats_.lower_bound() + stats_.upper_bound())/2 + 1);

    if(sat_solution_.empty())
    {
        stats_.add_upper_bound((stats_.lower_bound() + stats_.upper_bound())/2);
    }
    else
    {
        stats_.add_lower_bound(sat_solution_.shortest_segment());
    }

    if((stats_.gap() < 1) || (stats_.iteration > 20))
    {
        logger.stats(stats_);
        emit done();
    }
    else
    {
        ++stats_.iteration;
        QTimer::singleShot(0, this, SLOT(iteration()));
    }
}

void Controller::pre_solving()
{
    // shortest segment of convex hull must be part of triangulation
    stats_.add_upper_bound(convex_hull_.shortest_segment(segments_));

    // each non-intersected segment must be part of the triangulation
    stats_.add_upper_bound(intersection_graph_.shortest_segment());
}
