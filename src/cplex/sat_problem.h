#ifndef SAT_PROBLEM_H
#define SAT_PROBLEM_H

#include <vector>

#include "index_types.h"

#include "cplex/concert.h"
#include "cplex/sat_solution.h"

#include "intersection/types.h"

class SATProblem
{
public:
    SATProblem(SegmentIndex num_points,
               SegmentIndex num_segments,
               SegmentIndex num_convex_hull,
               const IntersectionGroupVector& igroups);

    const SATSolution &solve();
private:
    IloEnv           env_;
    IloModel         model_;
    IloBoolVarArray  variables_;

    SATSolution solution_;
};

#endif // SAT_PROBLEM_H
