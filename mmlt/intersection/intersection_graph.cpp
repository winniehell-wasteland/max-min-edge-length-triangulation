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
      SegmentData::IntersectionGroupIndices& igroups = segments_[segment_index].data().intersection_groups;
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

SegmentIndex IntersectionGraph::longest_intersecting_segment(const SegmentIndex& index) const
{
    SegmentIndex longest = index;
    const SegmentData::IntersectionGroupIndices igroups = segments_[index].data().intersection_groups;

    for(const IntersectionGroupIndex& igroup_index : igroups)
    {
        const IntersectionGroup& igroup = this->intersection_groups_[igroup_index];

        longest = std::max(longest, *std::max_element(igroup.begin(), igroup.end()));
    }

    return longest;
}

SegmentIndex IntersectionGraph::shortest_nonintersecting_segment() const
{
    for(const Segment& segment : segments_)
    {
        if(segment.data().index == longest_intersecting_segment(segment.data().index))
        {
            logger.info(mmlt_msg("shortest non-intersecting segment: %1 (len^2=%2)")
                        .arg(segment.to_string())
                        .arg(CGAL::to_double(segment.squared_length()), 0, 'f'));
            return segment.data().index;
        }
    }

    return segments_.size();
}
