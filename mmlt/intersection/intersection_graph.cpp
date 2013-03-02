#include "intersection_graph.h"

IntersectionGraph::IntersectionGraph(SegmentContainer& segments) :
    intersection_groups_(),
    segments_(segments)
{

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
