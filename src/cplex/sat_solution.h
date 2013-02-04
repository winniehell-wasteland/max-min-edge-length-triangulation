#ifndef MMT__CPLEX__SAT_SOLUTION_H
#define MMT__CPLEX__SAT_SOLUTION_H

#include <vector>

#include "cplex/concert.h"
#include "intersection/intersection_graph.h"

typedef std::vector<SegmentIndex>  SegmentIndexVector;

class SATSolution :
        public SegmentIndexVector
{
public:
    typedef SegmentIndexVector  base;

    SATSolution() :
        base()
    {

    }

    void draw(QPainter& painter, const SegmentContainer& segments) const;
    const SegmentIndex& shortest_segment() const;
};

#endif // MMT__CPLEX__SAT_SOLUTION_H
