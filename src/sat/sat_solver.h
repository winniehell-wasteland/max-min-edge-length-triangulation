#pragma once
#ifndef MMLT_SAT_SAT_SOLVER_H
#define MMLT_SAT_SAT_SOLVER_H

#include "config.h"

#include <QString>

#include "sat/sat_problem.h"
#include "sat/sat_solution.h"

#include "utils/settings.h"

namespace MMLT
{

/**
 * interface for SAT solvers
 */
class SATSolver
{
protected:
    virtual void add_forbidden_segment(const SATProblem* problem,
                                       const SegmentIndex& index) = 0;

    virtual void add_intersection_restrictions(const SATProblem* problem,
                                              const SegmentIndex& index,
                                              const Intersections& igroup) = 0;

    virtual void add_separation_restriction(const SATProblem* problem,
                                            const SegmentIndex& index,
                                            const std::vector<SegmentIndex>& separators) = 0;

    virtual void dump_problem(const QString& file_prefix,
                              const SATProblem* problem) = 0;

    void prepare_problem(const SATProblem& problem, bool complete);

    virtual void solve_problem(const SATProblem* problem,
                               SATSolution& solution) = 0;
};

/**
 * interface for decision SAT solvers
 */
class DecisionSATSolver :
        public SATSolver
{
public:
    void solve_decision_problem(const Settings& settings,
                                const QString& file_prefix,
                                const SATProblem& problem,
                                SATSolution& solution);
protected:
    virtual void init_decision_problem(const SATProblem* problem) = 0;
};

/**
 * interface for optimization SAT solvers
 */
class OptimizationSATSolver :
        public SATSolver
{
public:
    void solve_optimization_problem(const Settings& settings,
                                    const QString& file_prefix,
                                    const SATProblem& problem,
                                    SATSolution& solution);
protected:
    virtual void add_short_segment_restriction(const SATProblem* problem,
                                               const SegmentIndex& index) = 0;

    virtual void init_optimization_problem(const SATProblem* problem) = 0;
};

}

#endif // MMLT_SAT_SAT_SOLVER_H
