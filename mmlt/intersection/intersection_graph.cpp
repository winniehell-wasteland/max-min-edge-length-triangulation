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

  for(const SegmentIndex& segment_index : group)
  {
      IntersectionGroupIndices& igroups = segments_[segment_index].data().intersection_groups;
      igroups.insert(igroups.end(), group_index);
  }
}

void IntersectionGraph::draw(QPainter& painter) const
{
    for(const Segment& segment : segments_)
    {
        segment.draw(painter);
    }
}

SegmentIndex IntersectionGraph::shortest_nonintersecting_segment() const
{
    for(const Segment& segment : segments_)
    {
        SegmentIndex longest_intersecting_segment = segment.data().index;

        for(const IntersectionGroupIndex& igroup_index : segment.data().intersection_groups)
        {
            const IntersectionGroup& igroup = this->intersection_groups_[igroup_index];

            longest_intersecting_segment = std::max(longest_intersecting_segment,
                                                    *std::max_element(igroup.begin(), igroup.end()));
        }

        if(segment.data().index == longest_intersecting_segment)
        {
            logger.info(mmlt_msg("shortest non-intersecting segment: %1 (len^2=%2)")
                        .arg(segment.to_string())
                        .arg(CGAL::to_double(segment.squared_length())));
            return segment.data().index;
        }
    }

    return segments_.size();
}
