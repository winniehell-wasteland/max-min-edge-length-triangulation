#include "intersection_graph.h"

IntersectionGraph::IntersectionGraph(const SegmentIndex& size) :
    intersections_(size)
{

}

void IntersectionGraph::add_intersection(const Segment& s1, const Segment& s2)
{
    MMLT_precondition(s1.data().index < intersections_.size());
    MMLT_precondition(s2.data().index < intersections_.size());
    MMLT_precondition(s1.data().index < s2.data().index);

    intersections_[s1.data().index].insert(s2.data().index);
    intersections_[s2.data().index].insert(s1.data().index);
}

const SegmentIndex& IntersectionGraph::longest_intersecting_segment(const SegmentIndex& index) const
{
    MMLT_precondition(index < intersections_.size());
    const Intersections& igroup = intersections_[index];

    if(igroup.empty()) {
        return index;
    }
    else
    {
        return *std::max_element(igroup.begin(), igroup.end());
    }
}
