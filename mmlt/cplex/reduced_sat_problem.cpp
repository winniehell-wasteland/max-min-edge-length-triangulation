#include "utils/logger.h"

#include "reduced_sat_problem.h"

ReducedSATProblem::ReducedSATProblem(const IntersectionGraph& igraph,
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

    // index of minimum used segment
    IloIntVar penalty(cplex_.getEnv(),"penalty");
    model_.add(IloObjective(cplex_.getEnv(), penalty, IloObjective::Minimize));

    for(SegmentIndex segment_index = lower_bound; segment_index < upper_bound; ++segment_index)
    {
        const Segment& segment = segments[segment_index];

        // bind variable to objective
        IloIntExpr restr_bind_min(cplex_.getEnv());

        MMLT_precondition(segment_index < std::numeric_limits<IloInt>::max());
        restr_bind_min = IloInt(upper_bound-segment_index+1)*variables_[segment_index];
        restr_bind_min -= penalty;

        model_.add(restr_bind_min <= 0);

        // add choice: either segment or separator
        IloIntExpr restr_separators(cplex_.getEnv());
        restr_separators = variables_[segment_index];

        for(const IntersectionGroupIndex& igroup_index : segment.data().intersection_groups)
        {
            const IntersectionGroup& igroup = igraph[igroup_index];

            for(const SegmentIndex& separator_index : igroup)
            {
                if(separator_index <= segment_index)
                {
                    continue;
                }

                Separators::iterator separator;

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
                else
                {
                    separator = separators_.insert(
                        Separators::value_type(
                            separator_index,
                            IloBoolVar(cplex_.getEnv(), QString("s(%1)").arg(separator_index).toLocal8Bit().constData())
                        )
                    ).first;
                }

                restr_separators += separator->second;
            }
        }


        model_.add(restr_separators >= 1);
    }

    std::set<IntersectionGroupIndex> handled_igroups;

    // collect separator igroups
    for(auto separator : separators_)
    {
        const SegmentData::IntersectionGroupIndices& igroups = segments[separator.first].data().intersection_groups;

        // prevent intersecting separators
        for(const IntersectionGroupIndex& igroup_index : igroups)
        {
            if(!handled_igroups.insert(igroup_index).second)
            {
                // intersection group exists already
                continue;
            }

            const IntersectionGroup& igroup = igraph[igroup_index];
            IloExpr restr_intersection(cplex_.getEnv());

            bool insert_restr = false;

            // loop over intersected separators
            for(const SegmentIndex& segment_index : igroup)
            {
                auto it = separators_.find(segment_index);

                if(it != separators_.end())
                {
                    restr_intersection += it->second;

                    if(it->first != separator.first)
                    {
                        insert_restr = true;
                    }
                }
            }

            if(insert_restr)
            {
                model_.add(restr_intersection <= 1);
            }
        }
    }

    cplex_.extract(model_);
}

void ReducedSATProblem::solve(const QSettings& settings, const QString& file_prefix, SATSolution& solution)
{
    logger.info(mmlt_msg("Solving reduced SAT problem..."));

    if(settings.value("cplex/dump_models").toBool())
    {
        cplex_.exportModel(QString("%1_reduced.lp")
                           .arg(file_prefix)
                           .toLocal8Bit().data());
    }

    solution.clear();
    cplex_.solve();

    auto status = cplex_.getStatus();

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
