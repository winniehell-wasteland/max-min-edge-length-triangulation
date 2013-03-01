#include <QString>

#include "utils/logger.h"

#include "sat_problem.h"

SATProblem::SATProblem(SegmentIndex num_points,
                       SegmentIndex num_segments,
                       SegmentIndex num_convex_hull,
                       const IntersectionGraph& igraph) :
    cplex_(),
    model_(cplex_.getEnv(), "SAT"),
    variables_(cplex_.getEnv(), num_segments)
{
    variables_.setNames("x");

    // triangulation restriction
    model_.add(IloSum(variables_) == 3*num_points - num_convex_hull - 3);

    for(const IntersectionGroup& igroup : igraph)
    {
        IloExpr restr_intersection(cplex_.getEnv());

        foreach(SegmentIndex seg_index, igroup)
        {
            restr_intersection += variables_[seg_index];
        }

        model_.add(restr_intersection <= 1);
    }

    IloIntExpr objective(cplex_.getEnv(), 1);
    model_.add(IloObjective(cplex_.getEnv(), objective, IloObjective::Maximize));

    cplex_.extract(model_);
}

void SATProblem::solve(const QSettings& settings, const QString& file_prefix, SATSolution& solution, const SegmentIndex& lower_bound)
{
    logger.info(mmlt_msg("Solving SAT problem (lb=%1)...").arg(lower_bound));

    IloConstraintArray lb_restrictions(cplex_.getEnv());

    // disable variables
    for(SegmentIndex i = 0; i < lower_bound; ++i)
    {
        lb_restrictions.add(variables_[i] == 0);
    }
    model_.add(lb_restrictions);

    if(settings.value("cplex/dump_models").toBool())
    {
        cplex_.exportModel(QString("%1_%2.lp")
                           .arg(file_prefix)
                           .arg(lower_bound)
                           .toLocal8Bit().data());
    }

    solution.clear();
    cplex_.solve();

    auto status = cplex_.getStatus();

    switch (status) {
    case IloAlgorithm::Feasible:
        logger.debug(mmlt_msg("Found feasible solution"));
        break;
    case IloAlgorithm::Optimal:
        logger.debug(mmlt_msg("Found optimal solution"));
        break;
    case IloAlgorithm::Infeasible:
        logger.debug(mmlt_msg("Solution is infeasible"));
        break;
    case IloAlgorithm::Unbounded:
        logger.debug(mmlt_msg("Solution is unbounded"));
        break;
    case IloAlgorithm::InfeasibleOrUnbounded:
        logger.debug(mmlt_msg("Solution is infeasible or unbounded"));
        break;
    case IloAlgorithm::Unknown:
        logger.debug(mmlt_msg("Unknown solution status?!?"));
        break;
    case IloAlgorithm::Error:
        logger.debug(mmlt_msg("Error"));
        break;
    }

    if(status == IloAlgorithm::Optimal)
    {
        //logger.debug(mmlt_msg("Objective: %0").arg(cplex_.getObjValue()));

        for(SegmentIndex segment_index = 0;
            segment_index < variables_.getSize();
            ++segment_index)
        {
            logger.debug(mmlt_msg("%1 = %2")
                         .arg(variables_[segment_index].getName())
                         .arg(cplex_.getIntValue(variables_[segment_index])));

            if(cplex_.getValue(variables_[segment_index]) == true)
            {
                solution.push_back(segment_index);
            }
        }
    }

    // reset
    model_.remove(lb_restrictions);
    lb_restrictions.end();
}
