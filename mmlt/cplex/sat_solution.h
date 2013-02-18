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

    void draw(QPainter& painter, const SegmentContainer& segments) const;
    const SegmentIndex& shortest_segment() const;
};

#endif // MMLT_CPLEX_SAT_SOLUTION_H
