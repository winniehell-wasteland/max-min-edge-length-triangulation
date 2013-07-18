#include "utils/assertions.h"
#include "utils/logger.h"

#include "cplex_sat_solver.h"

namespace MMLT
{

CplexSATSolver::CplexSATSolver() :
    problem_data_()
{

}

void CplexSATSolver::add_forbidden_segment(const SATProblem* problem,
                                           const SegmentIndex& index)
{
    ProblemData& data = problem_data(problem);

    IloRange restriction = (data.variables_[index] == 0);
    restriction.setName(QString("forbidden_%1").arg(index).toLocal8Bit().data());

    data.model_.add(restriction);
}

void CplexSATSolver::add_intersection_restrictions(const SATProblem* problem,
                                                  const SegmentIndex& index,
                                                  const Intersections& intersections)
{
    ProblemData& data = problem_data(problem);

    for(const SegmentIndex& intersecting_index : intersections)
    {
        // we don't need intersections twice
        if(intersecting_index < index)
        {
            continue;
        }

        IloExpr sum(data.cplex_.getEnv());

        sum += data.variables_[index];
        sum += data.variables_[intersecting_index];

        // for every pair fo intersecting segments
        // only one can be picked
        IloRange restriction = (sum <= 1);
        restriction.setName(QString("intersection_%1_%2")
                            .arg(index)
                            .arg(intersecting_index)
                            .toLocal8Bit().data());

        data.model_.add(restriction);
    }
}

void CplexSATSolver::add_separation_restriction(const SATProblem* problem,
                                                const SegmentIndex& index,
                                                const std::vector<SegmentIndex>& separators)
{
    ProblemData& data = problem_data(problem);

    IloExpr sum(data.cplex_.getEnv());

    sum += data.variables_[index];

    for(const SegmentIndex& separator_index : separators)
    {
        sum += data.variables_[separator_index];
    }

    // at least one from the segment itself or its separators
    // has to be picked
    IloRange restriction = (sum >= 1);
    restriction.setName(QString("separation_%1").arg(index).toLocal8Bit().data());

    data.model_.add(restriction);
}

void CplexSATSolver::add_short_segment_restriction(const SATProblem* problem,
                                                   const SegmentIndex& index)
{
    ProblemData& data = problem_data(problem);

    // ensure the unsigned int fits in the signed int
    MMLT_precondition(problem->upper_bound() < std::numeric_limits<IloInt>::max());

    // bind variable to objective
    IloIntExpr sum(data.cplex_.getEnv());

    // the shorter the segment, the higher the penalty
    sum = IloInt(problem->upper_bound() - index) * data.variables_[index];
    sum -= data.objective_var_;

    IloRange restriction = (sum <= 0);
    restriction.setName(QString("short_%1").arg(index).toLocal8Bit().data());

    data.model_.add(restriction);
}

void CplexSATSolver::dump_problem(const QString& file_prefix,
                                   const SATProblem* problem)
{
    ProblemData& data = problem_data(problem);

    // whatever this does... it is necessary before calling exportModel()
    data.cplex_.extract(data.model_);

    QString file_name = QString("%1_%2-%3.lp")
            .arg(file_prefix)
            .arg(problem->lower_bound())
            .arg(problem->upper_bound());

    logger.info(mmlt_msg("Dumping SAT problem to %1...").arg(file_name));

    try
    {
        data.cplex_.exportModel(file_name.toLocal8Bit().data());
    }
    catch(IloCplex::Exception e)
    {
        logger.error(mmlt_msg("Could not extract model: %1").arg(e.getMessage()));
        MMLT_assertion(false);
    }
}

void CplexSATSolver::init_decision_problem(const SATProblem* problem)
{
    MMLT_precondition(problem != nullptr);
    MMLT_precondition(problem_data_.count(problem) == 0);

    auto entry = std::make_pair(problem, ProblemData(problem->size()));
    auto it = problem_data_.insert(entry).first;
    MMLT_postcondition(it->first == problem);

    ProblemData& data = it->second;

    // we just want any solution
    data.model_.add(data.objective_var_ == 1);
}

void CplexSATSolver::init_optimization_problem(const SATProblem* problem)
{
    MMLT_precondition(problem != nullptr);
    MMLT_precondition(problem_data_.count(problem) == 0);

    auto entry = std::make_pair(problem, ProblemData(problem->size()));
#ifndef NDEBUG
    auto it =
#endif
    problem_data_.insert(entry).first;
    MMLT_postcondition(it->first == problem);
}

void CplexSATSolver::solve_problem(const SATProblem* problem,
                                   SATSolution& solution)
{
    ProblemData& data = problem_data(problem);

    // whatever this does... it is necessary before calling solve()
    data.cplex_.extract(data.model_);

    solution.clear();
    data.cplex_.solve();

    auto status = data.cplex_.getStatus();

    switch (status) {
    case IloAlgorithm::Feasible:
        logger.debug(mmlt_msg("Found feasible solution"));
        break;
    case IloAlgorithm::Optimal:
        logger.debug(mmlt_msg("Found optimal solution"));
        break;
    case IloAlgorithm::Infeasible:
        logger.error(mmlt_msg("Solution is infeasible"));
        break;
    case IloAlgorithm::Unbounded:
        logger.error(mmlt_msg("Solution is unbounded"));
        break;
    case IloAlgorithm::InfeasibleOrUnbounded:
        logger.error(mmlt_msg("Solution is infeasible or unbounded"));
        break;
    case IloAlgorithm::Unknown:
        logger.error(mmlt_msg("Unknown solution status?!?"));
        break;
    case IloAlgorithm::Error:
        logger.error(mmlt_msg("Error"));
        break;
    }

    if(status == IloAlgorithm::Optimal)
    {
        logger.debug(mmlt_msg("Objective: %0").arg(data.cplex_.getObjValue()));

        for(SegmentIndex i = 0; i < data.variables_.getSize(); ++i)
        {
            if(!data.cplex_.isExtracted(data.variables_[i]))
            {
                /*
                logger.debug(mmlt_msg("%1 not part of SAT")
                             .arg(data.variables_[i].getName()));
                             */
                continue;
            }

            logger.debug(mmlt_msg("%1 = %2")
                         .arg(data.variables_[i].getName())
                         .arg(data.cplex_.getIntValue(data.variables_[i])));

            // add all set variables to the solution
            if(data.cplex_.getIntValue(data.variables_[i]) == true)
            {
                solution.push_back(i);
            }
        }
    }

    finish_problem(problem);
}

void CplexSATSolver::finish_problem(const SATProblem* problem)
{
    MMLT_precondition(problem != nullptr);

    auto it = problem_data_.find(problem);
    MMLT_postcondition(it != problem_data_.end());
    MMLT_postcondition(it->first == problem);

    problem_data_.erase(it);
}

CplexSATSolver::ProblemData& CplexSATSolver::problem_data(const SATProblem* problem)
{
    MMLT_precondition(problem != nullptr);

    auto it = problem_data_.find(problem);
    MMLT_postcondition(it != problem_data_.end());
    MMLT_postcondition(it->first == problem);

    return it->second;
}

}
