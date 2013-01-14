#ifndef MMT_CONTROLLER_H
#define MMT_CONTROLLER_H

#include "config.h"

#include <QFile>

#include "parameters.h"

#include "containers/convex_hull.h"
#include "containers/segment_container.h"

#include "intersection/intersection_graph.h"

class Controller
{
public:
    Controller(QFile &input_file, const Parameters& parameters);
    void start();
private:
    const PointSet points_;
    SegmentContainer segments_;

    const ConvexHull         convex_hull_;
    const IntersectionGraph  intersection_graph_;
    const Parameters&        parameters_;
};

#endif // MMT_CONTROLLER_H
