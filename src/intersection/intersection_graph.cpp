#include <CGAL/bounding_box.h>

#include <QPainter>
#include <QTextStream>

#include "mmt_utils.h"

#include "naive_intersection_algorithm.h"

//#include "arr_intersection_algorithm.h"
//#include "sweep_intersection_algorithm.h"

#include "intersection_graph.h"

/** select intersection algorithm */
// typedef SweepIntersectionAlgorithm  IntersectionAlgorithm;
// typedef ArrIntersectionAlgorithm  IntersectionAlgorithm;
typedef NaiveIntersectionAlgorithm  IntersectionAlgorithm;

const double IntersectionGraph::SCALE = 4.0;

IntersectionGraph::IntersectionGraph(const PointSet& points, SegmentContainer& segments) :
    bounding_box_(CGAL::bounding_box(points.begin(), points.end())),
    intersection_groups_(),
    segments_(segments)
{
    // find intersections
    IntersectionAlgorithm(this, points, segments);
}

void IntersectionGraph::add_intersection_group(const IntersectionGroup& group)
{
  intersection_groups_.push_back(group);
  IntersectionGroupIndex group_index = intersection_groups_.size() - 1;

  for(IntersectionGroup::const_iterator segment_index = group.begin();
      segment_index != group.end(); ++segment_index)
    {
      segments_[*segment_index].data().intersection_groups.push_back(group_index);
    }
}

void IntersectionGraph::draw_igraph(const QString& prefix) const
{
  QSvgGenerator generator;
  generator.setFileName(prefix + "_igraph.svg");

  init_generator(generator);

  QPainter painter;
  painter.begin(&generator);
  painter.scale(SCALE, SCALE);
  draw_graph(painter);
  painter.end();
}

void IntersectionGraph::draw_igroups(const QString& prefix) const
{
    IntersectionGroupIndex group_index = 0;

    for(IntersectionGroupVector::const_iterator group
        = intersection_groups_.begin();
        group != intersection_groups_.end(); ++group)
    {
        QSvgGenerator generator;
        generator.setFileName((prefix + "_igroup_%1.svg")
                              .arg(group_index, 5, 10, QChar('0')));

        init_generator(generator);

        QPainter painter;
        painter.begin(&generator);
        painter.scale(SCALE, SCALE);
        draw_graph(painter);
        draw_intersection_group(painter, *group);
        painter.end();

        ++group_index;
    }
}

void IntersectionGraph::init_generator(QSvgGenerator& generator) const
{
  generator.setSize(QSize(to_int(bounding_box_.xmax()) - to_int(bounding_box_.xmin()) + 20,
                          to_int(bounding_box_.ymax()) - to_int(bounding_box_.ymin()) + 20) * SCALE);

  generator.setViewBox(QRect(to_int(SCALE*(bounding_box_.xmin()-10)),
                             to_int(SCALE*(bounding_box_.ymin()-10)),
                             generator.size().width(),
                             generator.size().height()));
}

void IntersectionGraph::draw_graph(QPainter& painter) const
{
  QPen pen(QColor(0, 0, 0));
  pen.setWidthF(0.25);
  painter.setPen(pen);

  for(SegmentContainer::const_iterator segment = segments_.begin(); segment != segments_.end(); ++segment)
    {
      painter.drawLine(
            to_int(segment->source().x()),
            to_int(segment->source().y()),
            to_int(segment->target().x()),
            to_int(segment->target().y()));
    }
}

void IntersectionGraph::draw_intersection_group(QPainter& painter,
                                                const IntersectionGroup& group) const
{
  QPen pen(QColor(255, 0, 0));
  pen.setWidthF(0.25);
  painter.setPen(pen);

  for(IntersectionGroup::const_iterator segment_index = group.begin();
      segment_index != group.end(); ++segment_index)
    {
      painter.drawLine(
          to_int(segments_[*segment_index].source().x()),
          to_int(segments_[*segment_index].source().y()),
          to_int(segments_[*segment_index].target().x()),
          to_int(segments_[*segment_index].target().y()));
    }
}
