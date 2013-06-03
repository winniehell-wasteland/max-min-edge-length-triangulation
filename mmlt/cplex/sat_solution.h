#pragma once
#ifndef MMLT_CPLEX_SAT_SOLUTION_H
#define MMLT_CPLEX_SAT_SOLUTION_H

#include <vector>

#include "cplex/concert.h"
#include "intersection/intersection_graph.h"

class SATSolution :
        public std::vector<SegmentIndex>
{
public:
    SATSolution() :
        std::vector<SegmentIndex>()
    {

    }

    void draw_short_segments(QPainter& painter, const SegmentIndex& upper_bound, const SegmentContainer& segments) const;
    void draw_separators(QPainter& painter, const SegmentIndex& upper_bound, const SegmentContainer& segments) const;

    const SegmentIndex& shortest_segment(const SegmentIndex& upper_bound) const;
};

#endif // MMLT_CPLEX_SAT_SOLUTION_H
