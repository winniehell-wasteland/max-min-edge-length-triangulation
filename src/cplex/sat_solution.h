#ifndef MMT__CPLEX__SAT_SOLUTION_H
#define MMT__CPLEX__SAT_SOLUTION_H

#include <vector>

#include "index_types.h"

#include "cplex/concert.h"

#include "intersection/intersection_graph.h"

class SATSolution :
        public std::vector<SegmentIndex>
{
public:
    typedef std::vector<SegmentIndex>  base;

    IloAlgorithm::Status status;

    SATSolution() :
        base(),
        status(IloAlgorithm::Unknown)
    {

    }

    void clear()
    {
        base::clear();
        status = IloAlgorithm::Unknown;
    }

    void draw(QPainter& painter, const SegmentContainer& segments) const;
};

#endif // MMT__CPLEX__SAT_SOLUTION_H
