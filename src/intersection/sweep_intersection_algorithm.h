#ifndef MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H

#include <CGAL/Sweep_line_2.h>

#include "intersection_graph.h"
#include "sweep_line_visitor.h"

class SweepIntersectionAlgorithm
{
  typedef Sweep_line_visitor<SegmentTraits>           Visitor;
  typedef CGAL::Sweep_line_2<SegmentTraits, Visitor>  Sweep_line;
public:
  SweepIntersectionAlgorithm(IntersectionGraph& graph, const PointSet& points)
  {

    Visitor     visitor;
    Sweep_line  sweep_line(&visitor);
    visitor.sweep(graph.segments_begin(), graph.segments_end());

    logger->info(msg("Intersections: %1").arg(visitor.found_intersections()));
  }

private:
};

#endif // MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H
