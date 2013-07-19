#pragma once
#ifndef MMLT_CGAL_BOUNDING_BOX_H
#define MMLT_CGAL_BOUNDING_BOX_H

#include <CGAL/bounding_box.h>
#include <CGAL/Iso_rectangle_2.h>

#include "cgal/kernel.h"

#include "containers/point_set.h"

namespace MMLT
{

class BoundingBox :
        public CGAL::Iso_rectangle_2<Kernel>
{
public:
    BoundingBox( const PointSet& points ) :
        CGAL::Iso_rectangle_2<Kernel>(
            points.empty() ?
                CGAL::Iso_rectangle_2<Kernel>() :
                CGAL::bounding_box(points.begin(), points.end())
                )
    {

    }

    Number height() const
    {
        return (this->ymax() - this->ymin());
    }

    Number width() const
    {
        return (this->xmax() - this->xmin());
    }
};

}

#endif // MMLT_CGAL_BOUNDING_BOX_H
