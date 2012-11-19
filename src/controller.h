#ifndef MMT_CONTROLLER_H
#define MMT_CONTROLLER_H

#include "config.h"

#include "intersection/intersection_graph.h"

class Controller
{
public:
    Controller(const PointSet& points);
    void start();
private:
    const IntersectionGraph intersection_graph_;
    const PointSet& points_;
};

#endif // MMT_CONTROLLER_H
