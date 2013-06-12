#ifndef MMLT_CPLEX_CPLEX_SAT_SOLVER_H
#define MMLT_CPLEX_CPLEX_SAT_SOLVER_H

#include "config.h"

#include <map>

#include "cplex/concert.h"

#include "sat/sat_problem.h"
#include "sat/sat_solver.h"

class CplexSATSolver :
        public DecisionSATSolver,
        public OptimizationSATSolver
{
public:
    CplexSATSolver();

protected:
    virtual void add_forbidden_segment(const SATProblem* problem,
                                       const SegmentIndex& index) override;

    virtual void add_intersection_restrictions(const SATProblem* problem,
                                               const SegmentIndex& index,
                                               const Intersections& intersections) override;

    virtual void add_separation_restriction(const SATProblem* problem,
                                            const SegmentIndex& index,
                                            const std::vector<SegmentIndex>& separators) override;

    virtual void add_short_segment_restriction(const SATProblem* problem,
                                               const SegmentIndex& index) override;

    virtual void dump_problem(const QString& file_prefix,
                              const SATProblem* problem) override;

    virtual void init_decision_problem(const SATProblem* problem) override;

    virtual void init_optimization_problem(const SATProblem* problem) override;

    virtual void solve_problem(const SATProblem* problem,
                               SATSolution& solution) override;
private:
    struct ProblemData
    {
        CPLEX            cplex_;
        IloModel         model_;
        IloIntVar        objective_var_;
        IloBoolVarArray  variables_;

        ProblemData(const SegmentIndex& size) :
            cplex_(),
            model_(cplex_.getEnv(), "reduced SAT"),
            objective_var_(cplex_.getEnv(), "objective"),
            variables_(cplex_.getEnv(), size)
        {
            variables_.setNames("x");

            IloExpr objective_function = objective_var_;
            model_.add(IloObjective(cplex_.getEnv(), objective_function, IloObjective::Minimize));
        }
    };

    std::map<const SATProblem*, ProblemData>  problem_data_;

    void finish_problem(const SATProblem* problem);
    ProblemData& problem_data(const SATProblem* problem);
};

#endif // MMLT_CPLEX_CPLEX_SAT_SOLVER_H
