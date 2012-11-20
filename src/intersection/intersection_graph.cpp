#include <CGAL/bounding_box.h>

#include <QPainter>
#include <QTextStream>

#include "utils.h"

#include "arr_intersection_algorithm.h"
#include "naive_intersection_algorithm.h"
#include "sweep_intersection_algorithm.h"

#include "intersection_graph.h"

/** select intersection algorithm */
// typedef SweepIntersectionAlgorithm  IntersectionAlgorithm;
// typedef ArrIntersectionAlgorithm  IntersectionAlgorithm;
typedef NaiveIntersectionAlgorithm  IntersectionAlgorithm;

const double IntersectionGraph::SCALE = 4.0;

IntersectionGraph::IntersectionGraph(const PointSet& points) :
    IntersectionMap(CGAL::compare_to_less(SegmentOrder())),
    bounding_box_(CGAL::bounding_box(points.begin(), points.end())),
    intersection_groups_(),
    point_order_()
{
    IntersectionGroups empty;

    for(PointSet::const_iterator it = points.begin(); it != points.end(); ++it)
    {
        PointSet::const_iterator jt = it;
        for(++jt; jt != points.end(); ++jt)
        {
            CGAL_precondition_msg(point_order_(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");

            this->insert(std::make_pair(Segment(*it, *jt), empty));
        }
    }

    // find intersections
    IntersectionAlgorithm(this, points);
}

IntersectionGraph::mapped_type& IntersectionGraph::operator[] ( const IntersectionGraph::key_type& segment )
{
    CGAL_precondition_msg(point_order_(segment.source(), segment.target()) == CGAL::SMALLER, "Segment in wrong order!");

    return IntersectionMap::operator [](segment);
}

void IntersectionGraph::add_intersection_group(const IntersectionGroup& group)
{
  intersection_groups_.push_back(group);
  IntersectionGroupIndex group_index = intersection_groups_.size() - 1;

  for(IntersectionGroup::const_iterator segment = group.begin();
      segment != group.end(); ++segment)
    {
      (*this)[*segment].insert(group_index);
    }
}

void IntersectionGraph::draw(const QString& prefix,
                             const Parameters& parameters) const
{
  if(parameters.draw_igraph)
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

  if(parameters.draw_igroups)
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
}

void IntersectionGraph::remove_overlap(const Segment& segment)
{
  iterator pos = this->find(segment);

  for(IntersectionGroups::const_iterator group = pos->second.begin();
      group != pos->second.end(); ++group)
    {
      intersection_groups_[*group].erase(segment);
    }

  this->erase(pos);
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

  for(SegmentIterator segment = segments_begin(); segment != segments_end(); ++segment)
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

  for(IntersectionGroup::const_iterator segment = group.begin();
      segment != group.end(); ++segment)
    {
      painter.drawLine(
            to_int(segment->source().x()),
            to_int(segment->source().y()),
            to_int(segment->target().x()),
            to_int(segment->target().y()));
    }
}
