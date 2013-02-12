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
    foreach(const Segment& segment, segments_)
    {
        segment.draw(painter);
    }
}

void IntersectionGraph::draw_igroup(QPainter& painter, const IntersectionGroup& igroup) const
{
  foreach(SegmentIndex segment_index, igroup)
  {
      segments_[segment_index].draw(painter);
  }
}

SegmentIndex IntersectionGraph::shortest_segment() const
{
    for(SegmentIndex i = 0; i < segments_.size(); ++i)
    {
        if(segments_[i].data().intersection_groups.empty())
        {
            logger.info(mmt_msg("shortest non-intersecting segment: %1 (len^2=%2)")
                        .arg(segments_[i].to_string())
                        .arg(CGAL::to_double(segments_[i].squared_length())));
            return i;
        }
    }

    return segments_.size();
}
