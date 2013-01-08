#include <CGAL/bounding_box.h>

#include <QPainter>
#include <QTextStream>

#include "cgal_utils.h"

#include "naive_intersection_algorithm.h"

//#include "arr_intersection_algorithm.h"
//#include "sweep_intersection_algorithm.h"

#include "intersection_graph.h"

/** select intersection algorithm */
// typedef SweepIntersectionAlgorithm  IntersectionAlgorithm;
// typedef ArrIntersectionAlgorithm  IntersectionAlgorithm;
typedef NaiveIntersectionAlgorithm  IntersectionAlgorithm;

const double IntersectionGraph::SCALE = 4.0;

IntersectionGraph::IntersectionGraph(const PointSet& points) :
    bounding_box_(CGAL::bounding_box(points.begin(), points.end())),
    intersection_groups_(),
    segments_()
{
    SegmentIndex seg_index = 0;

    // allocate space
    //segments_.reserve(points.size()*(points.size() - 1)/2);

    // generate segments
    for(PointSet::const_iterator it = points.begin(); it != points.end(); ++it)
    {
        PointSet::const_iterator jt;
        for(jt = it, ++jt; jt != points.end(); ++jt)
        {
            CGAL_precondition_msg(point_order(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");

            Segment s(*it, *jt);
            s.data().index = seg_index;

            segments_.push_back(s);

            qDebug() << msg("Created segment %1 with index %2")
                        .arg(to_string(segments_.back()))
                        .arg(segments_.back().data().index);

            ++seg_index;
        }
    }

    CGAL_postcondition(seg_index == segments_.size());

    // find intersections
    IntersectionAlgorithm(this, points);
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

void IntersectionGraph::draw(const QString& prefix,
                             const Parameters& parameters) const
{
  if(parameters.draw_igraph)
    {
      qWarning() << msg("Drawing intersection graph...");

      QSvgGenerator generator;
      generator.setFileName(prefix + "_igraph.svg");

      init_generator(generator);

      QPainter painter;
      painter.begin(&generator);
      painter.scale(SCALE, SCALE);
      draw_graph(painter);
      painter.end();
    }

  if(parameters.draw_igroups)
    {
      qWarning() << msg("Drawing intersection groups...");

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

  for(SegmentVector::const_iterator segment = segments_.begin(); segment != segments_.end(); ++segment)
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
