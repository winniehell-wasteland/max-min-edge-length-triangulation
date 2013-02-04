#include <QString>

#include "utils/logger.h"

#include "sat_problem.h"

SATProblem::SATProblem(SegmentIndex num_points,
                       SegmentIndex num_segments,
                       SegmentIndex num_convex_hull,
                       const IntersectionGroupVector& igroups) :
    env_(),
    model_(env_, "SAT"),
    variables_(env_)
{
    QString var_name("x_%1");

    for(SegmentIndex seg_index = 0; seg_index < num_segments; ++seg_index)
    {
        variables_.add(IloBoolVar(env_, var_name.arg(seg_index).toLocal8Bit().constData()));
    }

    model_.add(IloSum(variables_) == 3*num_points - num_convex_hull - 3);

    foreach(IntersectionGroup igroup, igroups)
    {
        IloExpr restr_intersection(env_);

        foreach(SegmentIndex seg_index, igroup)
        {
            restr_intersection += variables_[seg_index];
        }

        model_.add(restr_intersection <= 1);
    }

    IloIntExpr objective(env_, 1);
    model_.add(IloObjective(env_, objective, IloObjective::Maximize));
}

void SATProblem::solve(SATSolution& solution, const SegmentIndex& lower_bound)
{
    // disable variables
    for(SegmentIndex i = 0; i < lower_bound; ++i)
    {
        variables_[i].setMax(0);
    }

    IloCplex cplex(model_);
    cplex.setOut(env_.getNullStream());
    cplex.setError(env_.getNullStream());
    cplex.setParam(IloCplex::RootAlg, IloCplex::AutoAlg);
    cplex.setParam(IloCplex::SimDisplay, 2);
    cplex.solve();

    solution.clear();
    auto status = cplex.getStatus();

    switch (status) {
    case IloAlgorithm::Feasible:
        logger.debug(mmt_msg("Found feasible solution"));
        break;
    case IloAlgorithm::Optimal:
        logger.debug(mmt_msg("Found optimal solution"));
        break;
    case IloAlgorithm::Infeasible:
        logger.debug(mmt_msg("Solution is infeasible"));
        break;
    case IloAlgorithm::Unbounded:
        logger.debug(mmt_msg("Solution is unbounded"));
        break;
    case IloAlgorithm::InfeasibleOrUnbounded:
        logger.debug(mmt_msg("Solution is infeasible or unbounded"));
        break;
    case IloAlgorithm::Unknown:
        logger.debug(mmt_msg("Unknown solution status?!?"));
        break;
    case IloAlgorithm::Error:
        logger.debug(mmt_msg("Error"));
        break;
    }

    if(status == IloAlgorithm::Optimal)
    {
        logger.debug(mmt_msg("Objective: %0").arg(cplex.getObjValue()));

        for(SegmentIndex segment_index = 0;
            segment_index < variables_.getSize();
            ++segment_index)
        {
            logger.debug(mmt_msg("%0 = %1")
                         .arg(variables_[segment_index].getName())
                         .arg(cplex.getValue(variables_[segment_index])));

            if(cplex.getValue(variables_[segment_index]))
            {
                solution.push_back(segment_index);
            }
        }
    }

    // reset
    for(SegmentIndex i = 0; i < lower_bound; ++i)
    {
        variables_[i].setMax(1);
    }
}
