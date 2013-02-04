#include "intersection/intersection_algorithm.h"

#include "intersection_graph.h"

IntersectionGraph::IntersectionGraph(const PointSet& points, SegmentContainer& segments) :
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

  foreach(SegmentIndex segment_index, group)
  {
      segments_[segment_index].data().intersection_groups.push_back(group_index);
  }
}

void IntersectionGraph::draw_igraph(QPainter& painter) const
{
    QPen pen(QColor(100, 100, 100));
    pen.setWidthF(1.0/MMT_SVG_SCALE);
    painter.setPen(pen);

    foreach(const Segment& segment, segments_)
    {
        segment.draw(painter);
    }
}

void IntersectionGraph::draw_igroup(QPainter& painter, const IntersectionGroup& igroup) const
{
  QPen pen(QColor(255, 0, 0));
  pen.setWidthF(1.0/MMT_SVG_SCALE);
  painter.setPen(pen);

  foreach(SegmentIndex segment_index, igroup)
  {
      segments_[segment_index].draw(painter);
  }
}

SegmentIndex IntersectionGraph::shortest_segment() const
{
    SegmentIndex min_segment = segments_.size() - 1;

    foreach(auto igroup, intersection_groups_)
    {
        foreach(SegmentIndex segment_index, igroup)
        {
            if(segment_index < min_segment)
            {
                min_segment = segment_index;
            }
        }
    }

    logger.info(mmt_msg("shortest non-intersecting segment: %1").arg(min_segment));

    return min_segment;
}
