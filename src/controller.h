#ifndef MMT_CONTROLLER_H
#define MMT_CONTROLLER_H

#include "config.h"

#include <QFile>
#include <QSvgGenerator>

#include "parameters.h"
#include "stats.h"
#include "containers/convex_hull.h"
#include "containers/segment_container.h"
#include "intersection/intersection_graph.h"

class Controller :
        public QObject
{
    Q_OBJECT
public:
    Controller(QFile& input_file, const Parameters& parameters);
public Q_SLOTS:
    void init();
    void iteration();
Q_SIGNALS:
    void done();
private:
    const QString                        file_prefix_;
    const Parameters&                    parameters_;
    const PointSet                       points_;
    Stats                                stats_;
    QSvgGenerator                        svg_generator_;

    const CGAL::Iso_rectangle_2<Kernel>  bounding_box_;
    const ConvexHull                     convex_hull_;
    SegmentContainer                     segments_;

    const IntersectionGraph              intersection_graph_;

    class SVGPainter :
            public QPainter
    {
    public:
        SVGPainter(Controller* controller, const QString& file_name) :
            QPainter()
        {
            controller->svg_generator_.setFileName(controller->file_prefix_ + file_name);
            this->begin(&controller->svg_generator_);
            this->scale(MMT_SVG_SCALE, MMT_SVG_SCALE);
        }
    };
};

#endif // MMT_CONTROLLER_H
