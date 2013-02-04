#ifndef MMT__CPLEX__SAT_PROBLEM_H
#define MMT__CPLEX__SAT_PROBLEM_H

#include "config.h"
#include "cplex/concert.h"
#include "cplex/sat_solution.h"
#include "intersection/intersection_group.h"

class SATProblem
{
public:
    SATProblem(SegmentIndex num_points,
               SegmentIndex num_segments,
               SegmentIndex num_convex_hull,
               const IntersectionGroupVector& igroups);

    void solve(SATSolution& solution, const SegmentIndex& lower_bound);
private:
    IloEnv           env_;
    IloModel         model_;
    IloBoolVarArray  variables_;
};

#endif // MMT__CPLEX__SAT_PROBLEM_H
