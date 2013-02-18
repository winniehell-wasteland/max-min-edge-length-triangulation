#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_GROUP_H
#define MMLT_INTERSECTION_INTERSECTION_GROUP_H

#include "config.h"

#include <set>
#include <vector>

#include <QPainter>

#include "containers/segment_container.h"

/**
 * sorted set of intersecting segments
 */
class IntersectionGroup :
        public std::set<SegmentIndex>
{
public:
    /** default constructor */
    IntersectionGroup() :
        std::set<SegmentIndex>()
    {

    }

    /** draw intersection group using QPainter */
    void draw(QPainter& painter, const SegmentContainer& segments) const;
    /** output intersection group to QString */
    QString to_string(const SegmentContainer& segments) const;
};

#endif // MMLT_INTERSECTION_INTERSECTION_GROUP_H

