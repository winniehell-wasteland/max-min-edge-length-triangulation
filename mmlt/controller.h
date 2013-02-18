#pragma once
#ifndef MMLT_CONTROLLER_H
#define MMLT_CONTROLLER_H

#include "config.h"

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QSvgGenerator>
#include <QTime>

#include "stats.h"
#include "containers/convex_hull.h"
#include "containers/segment_container.h"
#include "cplex/sat_problem.h"
#include "cplex/sat_solution.h"
#include "intersection/intersection_graph.h"

class Controller :
        public QObject
{
    Q_OBJECT
public:
    Controller(QCoreApplication& application, const QSettings& settings);
    void draw_igroups();
public Q_SLOTS:
    void iteration();
    void start();
private:
    const QTime                          start_time_;
    QCoreApplication&                    application_;
    QFile                                input_file_;

    const QString                        file_prefix_;
    const PointSet                       points_;
    const QSettings&                     settings_;
    Stats                                stats_;

    const CGAL::Iso_rectangle_2<Kernel>  bounding_box_;
    const ConvexHull                     convex_hull_;
    SegmentContainer                     segments_;

    const IntersectionGraph              intersection_graph_;
    SATProblem                           sat_problem_;
    SATSolution                          sat_solution_;

    class SVGPainter :
            public QPainter
    {
    public:
        SVGPainter(Controller* controller, const QString& file_name) :
            QPainter(),
            generator_(),
            pen_()
        {
            generator_.setFileName(controller->file_prefix_ + file_name);
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

    void draw_bounds();
    void draw_sat_solution();
    void pre_solving();
};

#endif // MMLT_CONTROLLER_H
