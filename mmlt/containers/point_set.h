#pragma once
#ifndef MMLT_CONTAINERS_POINT_SET_H
#define MMLT_CONTAINERS_POINT_SET_H

#include <set>

#include <QFile>

#include "cgal/kernel.h"
#include "cgal/orders.h"

/** (sorted) set of points */
class PointSet :
        public std::set<Point, STLPointOrder>
{
public:
    PointSet(QFile& input_file);
};

#endif // MMLT_CONTAINERS_POINT_SET_H
