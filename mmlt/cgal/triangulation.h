#pragma once
#ifndef MMLT_CGAL_TRIANGULATION_H
#define MMLT_CGAL_TRIANGULATION_H

#include "config.h"

#include <CGAL/Constrained_triangulation_2.h>

#include "cgal/kernel.h"

#include "containers/point_set.h"

class Triangulation :
        public CGAL::Constrained_triangulation_2<Kernel>
{
public:
    /** default constructor */
    Triangulation(const PointSet& points)
    {
        this->insert(points.begin(), points.end());
    }
};

#endif // MMLT_CGAL_TRIANGULATION_H
