#pragma once
#ifndef MMLT_CONTROLLER_H
#define MMLT_CONTROLLER_H

#include "config.h"

#include <QCoreApplication>
#include <QFile>
#include <QElapsedTimer>
#include <QSettings>
#include <QSvgGenerator>

#include "stats.h"

#include "cgal/triangulation.h"

#include "containers/convex_hull.h"
#include "containers/segment_container.h"

#include "cplex/sat_problem.h"
#include "cplex/sat_solution.h"

#include "intersection/intersection_algorithm.h"
#include "intersection/intersection_graph.h"

#include "utils/abort_timer.h"
#include "utils/elapsed_timer.h"

class Controller :
        public QObject
{
    Q_OBJECT
public:
    Controller(QCoreApplication& application, const QSettings& settings);
public Q_SLOTS:
    void iteration();
    void start();
private:
    using BoundingBox = CGAL::Iso_rectangle_2<Kernel>;

    /**
     * @name independent members
     * @{
     */
    ElapsedTimer                         timer_;
    SATSolution                          sat_solution_;
    Stats                                stats_;
    /**
     * @}
     */

    /**
     * @name input parameters
     * @{
     */
    QCoreApplication&                    application_;
    QFile                                input_file_;
    const QSettings&                     settings_;
    /**
     * @}
     */

    /**
     * @name dependent on input parameter
     * @{
     */
    AbortTimer                           abort_timer_;
    QString                              file_prefix_;
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
    IntersectionAlgorithm                intersection_algorithm_;
    IntersectionGraph                    intersection_graph_;
    /**
     * @}
     */

    /**
     * @name dependent on intersection graph
     * @{
     */
    // find intersections
    /**
     * @}
     */

    class SVGPainter :
            public QPainter
    {
    public:
        SVGPainter(const Controller* controller, const QString& file_name) :
            QPainter(),
            generator_(),
            pen_()
        {
            generator_.setFileName(controller->file_prefix_ + "_" + file_name);
            generator_.setTitle(controller->file_prefix_);

            generator_.setSize(QSize(
                CGAL::to_double(controller->bounding_box_.xmax())
                    - CGAL::to_double(controller->bounding_box_.xmin()) + 20,
                CGAL::to_double(controller->bounding_box_.ymax())
                    - CGAL::to_double(controller->bounding_box_.ymin()) + 20) * SVG_SCALE
            );

            generator_.setViewBox(QRect(
                CGAL::to_double(SVG_SCALE*(controller->bounding_box_.xmin()-10)),
                CGAL::to_double(SVG_SCALE*(controller->bounding_box_.ymin()-10)),
                generator_.size().width(), generator_.size().height())
            );

            this->begin(&generator_);

            pen_.setWidthF(0.5);
            pen_.setCosmetic(true);
            this->setPen(pen_);

            this->scale(SVG_SCALE, SVG_SCALE);
        }

        void setPenColor(const QColor& color)
        {
            pen_.setColor(color);
            this->setPen(pen_);
        }

        ~SVGPainter()
        {
            this->end();
        }

    private:
        /** scale for SVG images */
        static const double SVG_SCALE;

        QSvgGenerator generator_;
        QPen          pen_;
    };

    void done() const;

    void draw_bounds() const;
    void draw_igroups() const;
    void draw_points(SVGPainter& painter) const;
    void draw_sat_solution() const;
    void draw_segments(SVGPainter& painter) const;
    void draw_separators() const;

    void output_status() const;
    void pre_solving();
};

#endif // MMLT_CONTROLLER_H
