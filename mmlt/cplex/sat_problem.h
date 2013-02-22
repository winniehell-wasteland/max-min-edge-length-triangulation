#pragma once
#ifndef MMLT_CPLEX_SAT_PROBLEM_H
#define MMLT_CPLEX_SAT_PROBLEM_H

#include "config.h"

#include <QSettings>

#include "cplex/concert.h"
#include "cplex/sat_solution.h"

#include "intersection/intersection_graph.h"

class SATProblem
{
public:
    SATProblem(SegmentIndex num_points,
               SegmentIndex num_segments,
               SegmentIndex num_convex_hull,
               const IntersectionGraph& igraph);

    void solve(const QSettings& settings, const QString& file_prefix, SATSolution& solution, const SegmentIndex& lower_bound);
private:
    IloEnv           env_;

    IloCplex         cplex_;
    IloModel         model_;
    IloBoolVarArray  variables_;
};

#endif // MMLT_CPLEX_SAT_PROBLEM_H
