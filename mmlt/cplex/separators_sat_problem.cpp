#include "utils/logger.h"

#include "separators_sat_problem.h"

SeparatorsSATProblem::SeparatorsSATProblem(const IntersectionGraph& igraph,
                       const SegmentContainer& segments,
                       const SegmentIndex& lower_bound,
                       const SegmentIndex& upper_bound) :
    igraph_(igraph),
    cplex_(),
    model_(cplex_.getEnv(), "reduced SAT"),
    separators_(),
    variables_(cplex_.getEnv(), upper_bound)
{
    variables_.setNames("x");

    // igroups that lead to restrictions
    std::set<IntersectionGroupIndex> restricting_igroups;

    // penalty for using short segments
    IloIntVar penalty(cplex_.getEnv(),"penalty");
    model_.add(IloObjective(cplex_.getEnv(), penalty, IloObjective::Minimize));

    for(SegmentIndex segment_index = lower_bound; segment_index < upper_bound; ++segment_index)
    {
        const Segment& segment = segments[segment_index];

        // bind variable to objective
        IloIntExpr restr_bind_penalty(cplex_.getEnv());

        // ensure the unsigned int fits in the signed int
        MMLT_precondition(segment_index < std::numeric_limits<IloInt>::max());

        // the shorter the segment, the higher the penalty
        restr_bind_penalty = IloInt(upper_bound-segment_index) * variables_[segment_index];
        restr_bind_penalty -= penalty;

        model_.add(restr_bind_penalty <= 0);

        // add choice: either segment or separator
        IloIntExpr restr_separators(cplex_.getEnv());
        restr_separators = variables_[segment_index];

        for(const IntersectionGroupIndex& igroup_index : segment.data().intersection_groups)
        {
            restricting_igroups.insert(igroup_index);

            const IntersectionGroup& igroup = igraph[igroup_index];

            for(const SegmentIndex& separator_index : igroup)
            {
                // separators must be longer than segment
                // (i.e. higher index in segment set)
                if(separator_index <= segment_index)
                {
                    continue;
                }

                Separators::iterator separator;

                // if separator is a short segment, we already have a variable
                if((lower_bound <= separator_index)
                        && (separator_index < upper_bound))
                {

                    separator = separators_.insert(
                        Separators::value_type(
                            separator_index,
                            variables_[separator_index]
                        )
                    ).first;
                }
                // if separator is a long segment, add a variable to
                // the separators map (there is only one variable for
                // each long segment, duplicates don't get inserted)
                else
                {
                    separator = separators_.insert(
                        Separators::value_type(
                            separator_index,
                            IloBoolVar(cplex_.getEnv(),
                                       QString("y[%1]")
                                       .arg(separator_index)
                                       .toLocal8Bit().constData()
                                       )
                        )
                    ).first;
                }

                restr_separators += separator->second;
            }
        }

        // at least one from the segment itself or its separators
        // has to be picked
        model_.add(restr_separators >= 1);
    }


    // collect separator igroups
    for(auto separator : separators_)
    {
        const SegmentData::IntersectionGroupIndices& igroups =
                segments[separator.first].data().intersection_groups;

        restricting_igroups.insert(igroups.begin(), igroups.end());
    }

    // prevent intersecting segments
    for(const IntersectionGroupIndex& igroup_index : restricting_igroups)
    {
        const IntersectionGroup& igroup = igraph[igroup_index];
        MMLT_assertion(igroup.size() == 2);

        IloExpr restr_intersection(cplex_.getEnv());

        // loop over intersecting segments
        for(const SegmentIndex& segment_index : igroup)
        {
            if((lower_bound <= segment_index) && (segment_index < upper_bound))
            {
                restr_intersection += variables_[segment_index];
            }
            else
            {
                auto it = separators_.find(segment_index);

                // segment is a separator
                if(it != separators_.end())
                {
                    // add separator variable to restriction
                    restr_intersection += it->second;
                }
                else
                {
                    // the segment is neither a short segment nor
                    // a separator, so just ignore it
                }
            }
        }

        // from all the segments intersecting in one point
        // only one can be picked
        model_.add(restr_intersection <= 1);
    }

    // whatever this does... it is necessary to call solve()
    cplex_.extract(model_);
}

void SeparatorsSATProblem::solve(const QSettings& settings, const QString& file_prefix, SATSolution& solution)
{
    logger.info(mmlt_msg("Solving reduced SAT problem..."));

    if(settings.value("cplex/dump_models").toBool())
    {
        logger.info(mmlt_msg("Dumping SAT problem to %1.lp...").arg(file_prefix));
        cplex_.exportModel(QString("%1.lp")
                           .arg(file_prefix)
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
        logger.debug(mmlt_msg("Objective: %0").arg(cplex_.getObjValue()));

        for(SegmentIndex segment_index = 0;
            segment_index < variables_.getSize();
            ++segment_index)
        {
            logger.debug(mmlt_msg("%1 = %2")
                         .arg(variables_[segment_index].getName())
                         .arg(cplex_.getIntValue(variables_[segment_index])));

            if(cplex_.getIntValue(variables_[segment_index]) == true)
            {
                solution.push_back(segment_index);
            }
        }

        for(auto separator : separators_)
        {
            logger.debug(mmlt_msg("%1 = %2")
                         .arg(separator.second.getName())
                         .arg(cplex_.getIntValue(separator.second)));

            if(cplex_.getIntValue(separator.second) == true)
            {
                solution.push_back(separator.first);
            }
        }
    }
}
