#pragma once
#ifndef MMLT_CONTAINERS_POINT_SET_H
#define MMLT_CONTAINERS_POINT_SET_H

#include <set>

#include <QFile>
#include <QPainter>

#include "cgal/kernel.h"
#include "cgal/orders.h"

/** (sorted) set of points */
class PointSet :
        public std::set<Point, STLPointOrder>
{
public:
    /**
     * empty set
     */
    PointSet();

    /**
     * read points from file
     */
    PointSet(QFile& input_file);

    bool contains(const Point& point) const
    {
        return (this->count(point) > 0);
    }

    /** output point set using QPainter */
    void draw(QPainter& painter) const;
};

#endif // MMLT_CONTAINERS_POINT_SET_H
