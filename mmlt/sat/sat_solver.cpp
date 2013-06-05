#include "intersection/intersections.h"

#include "utils/assertions.h"
#include "utils/logger.h"

#include "sat_solver.h"

void SATSolver::prepare_problem(const SATProblem& problem)
{
    const SegmentContainer& segments = problem.segments();

    std::vector<SegmentIndex> separators;

    // forbid unnecessary segments
    for(SegmentIndex segment_index = 0;
        segment_index < problem.lower_bound();
        segment_index++)
    {
        add_forbidden_segment(&problem, segment_index);
    }

    for(SegmentIndex segment_index = problem.lower_bound();
        segment_index < problem.upper_bound();
        ++segment_index)
    {
        const Segment& segment = segments[segment_index];

        // we don't want outer segments
        if(segment.data().is_outer)
        {
            add_forbidden_segment(&problem, segment_index);
            continue;
        }

        const Intersections& intersections = problem.intersections(segment_index);

        add_intersection_restrictions(&problem, segment_index, intersections);

        intersections.find_separators(segment_index, problem.segments(), separators);
        add_separation_restriction(&problem, segment_index, separators);

        // handle intersecting separators
        for(const SegmentIndex& separator_index : separators)
        {
            const Intersections& separator_intersections = problem.intersections(separator_index);
            add_intersection_restrictions(&problem, separator_index, separator_intersections);
        }

        separators.clear();
    }
}

void DecisionSATSolver::solve_decision_problem(const QSettings& settings,
                                               const QString& file_prefix,
                                               const SATProblem& problem,
                                               SATSolution& solution)
{
    init_decision_problem(&problem);
    prepare_problem(problem);

    if(settings.value("sat/dump_problems").toBool())
    {
        dump_problem(file_prefix, &problem);
    }

    logger.info(mmlt_msg("Solving SAT problem..."));
    solve_problem(&problem, solution);
}

void OptimizationSATSolver::solve_optimization_problem(const QSettings& settings,
                                                       const QString& file_prefix,
                                                       const SATProblem& problem,
                                                       SATSolution& solution)
{
    init_optimization_problem(&problem);
    prepare_problem(problem);

    // bind short segments to objective function
    for(SegmentIndex segment_index = problem.lower_bound();
        segment_index < problem.upper_bound();
        ++segment_index)
    {
        add_short_segment_restriction(&problem, segment_index);
    }

    if(settings.value("sat/dump_problems").toBool())
    {
        dump_problem(file_prefix, &problem);
    }

    logger.info(mmlt_msg("Solving SAT problem..."));
    solve_problem(&problem, solution);
}
