#include "sat_problem.h"

namespace MMLT
{

SATProblem::SATProblem(const IntersectionGraph& igraph,
                       const SegmentContainer& segments) :
    igraph_(igraph),
    segments_(segments),
    lower_bound_(0),
    upper_bound_(segments_.size())
{

}

void SATProblem::set_short_segment_range(const SegmentIndex& lower_bound,
                                         const SegmentIndex& upper_bound)
{
    lower_bound_ = lower_bound;
    upper_bound_ = upper_bound;
}

}
