#pragma once
#ifndef MMLT_CGAL_TRIANGULATION_H
#define MMLT_CGAL_TRIANGULATION_H

#include "config.h"

#include <CGAL/Delaunay_triangulation_2.h>

#include "cgal/kernel.h"

#include "containers/point_set.h"
#include "containers/segment_container.h"

class DelaunayTriangulation :
        public CGAL::Delaunay_triangulation_2<Kernel>
{
public:
    /** default constructor */
    DelaunayTriangulation(const PointSet& points)
    {
        this->insert(points.begin(), points.end());
    }

    /** find the shortest segment */
    SegmentIndex shortest_segment(const SegmentContainer& segments) const;
};

#endif // MMLT_CGAL_TRIANGULATION_H
