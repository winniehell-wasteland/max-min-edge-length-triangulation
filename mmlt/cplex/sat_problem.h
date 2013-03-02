#pragma once
#ifndef MMLT_CPLEX_SAT_PROBLEM_H
#define MMLT_CPLEX_SAT_PROBLEM_H

#include <map>

#include <QSettings>

#include "cplex/concert.h"
#include "cplex/sat_solution.h"

#include "containers/segment_container.h"

#include "intersection/intersection_graph.h"

class SATProblem
{
public:
    SATProblem(const IntersectionGraph& igraph,
                      const SegmentContainer& segments,
                      const SegmentIndex& lower_bound,
                      const SegmentIndex& upper_bound);

    void solve(const QSettings& settings, const QString& file_prefix, SATSolution& solution);
private:
    using Separators = std::map<SegmentIndex, IloBoolVar>;

    const IntersectionGraph& igraph_;

    CPLEX            cplex_;
    IloModel         model_;
    Separators       separators_;
    IloBoolVarArray  variables_;
};

#endif // MMLT_CPLEX_SAT_PROBLEM_H
