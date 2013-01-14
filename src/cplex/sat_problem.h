#ifndef SAT_PROBLEM_H
#define SAT_PROBLEM_H

#include "config.h"

#include "intersection/intersection_graph.h"

class SATProblem
{
public:
    SATProblem(SegmentIndex num_segments,
               SegmentIndex num_convex_hull,
               const IntersectionGraph& igraph);
};

#endif // SAT_PROBLEM_H
