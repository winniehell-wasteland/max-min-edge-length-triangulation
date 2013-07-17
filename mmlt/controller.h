#pragma once
#ifndef MMLT_CONTROLLER_H
#define MMLT_CONTROLLER_H

#include "config.h"

#include <QFile>
#include <QSettings>

#include "stats.h"

#include "cgal/bounding_box.h"

#include "containers/convex_hull.h"
#include "containers/segment_container.h"
#include "containers/triangulation.h"

#include "cplex/cplex_sat_solver.h"

#include "intersection/intersection_algorithm.h"
#include "intersection/intersection_graph.h"

#include "sat/sat_solution.h"

#include "utils/svg_painter.h"

class Controller
{
public:
    Controller(const QString& file_prefix,
               QFile& input_file,
               const QSettings& settings);

    /**
     * called after the algorithm finished
     */
    void done();

    /**
     * run next iteration
     * @return true if next iteration should be triggered
     */
    bool iteration();


    /**
     * start the algorithm
     * @return true if iteration should be triggered
     */
    bool start();
private:
    /**
     * @name independent members
     * @{
     */
    CplexSATSolver                       cplex_solver_;
    IntersectionAlgorithm                intersection_algorithm_;
    SATSolution                          last_sat_solution_;
    Stats                                stats_;
    /**
     * @}
     */

    /**
     * @name input parameters
     * @{
     */
    const QString&                       file_prefix_;
    QFile&                               input_file_;
    const QSettings&                     settings_;
    /**
     * @}
     */

    /**
     * @name dependent on input parameter
     * @{
     */
    const PointSet                       points_;
    /**
     * @}
     */

    /**
     * @name dependent on input points
     * @{
     */
    const BoundingBox                    bounding_box_;
    const ConvexHull                     convex_hull_;
    SegmentContainer                     segments_;
    Triangulation                        triangulation_;
    /**
     * @}
     */

    /**
     * @name dependent on segments
     * @{
     */
    IntersectionGraph                    intersection_graph_;
    /**
     * @}
     */

    void draw_bounds() const;
    void draw_intersections() const;
    void draw_points(SVGPainter& painter) const;
    void draw_sat_solution() const;
    void draw_segments(SVGPainter& painter) const;
    void draw_separators() const;
    void draw_triangulation() const;

    /**
     * dumps the current algorithm status
     */
    void output_status() const;

    /**
     * does some pre-processing
     */
    void pre_solving();
};

#endif // MMLT_CONTROLLER_H
