#pragma once
#ifndef MMLT_CGAL_TRIANGULATION_H
#define MMLT_CGAL_TRIANGULATION_H

#include "config.h"

#include <CGAL/Constrained_triangulation_2.h>

#include "cgal/kernel.h"

#include "containers/point_set.h"
#include "containers/segment_container.h"

class Triangulation :
        public CGAL::Constrained_triangulation_2<Kernel>
{
public:
    /**
     * default constructor
     */
    Triangulation(const PointSet& points);

    /**
     * draw triangulation segments using given QPainter
     */
    void draw(QPainter& painter) const;

    const SegmentIndex& shortest_segment(const SegmentContainer& segments) const;
};

#endif // MMLT_CGAL_TRIANGULATION_H
