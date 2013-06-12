#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTIONS_H
#define MMLT_INTERSECTION_INTERSECTIONS_H

#include "config.h"

#include <set>
#include <vector>

#include <QPainter>

#include "containers/segment_container.h"

/**
 * sorted set of intersecting segments
 */
class Intersections :
        public std::set<SegmentIndex>
{
public:
    /** default constructor */
    Intersections() :
        std::set<SegmentIndex>()
    {

    }

    /** draws intersections using QPainter */
    void draw(QPainter& painter, const SegmentContainer& segments) const;

    /**
     * finds all separators for a given segment
     * and stores them in the passed container
     */
    void find_separators(const SegmentIndex& segment_index,
                         const SegmentContainer& segments,
                         std::vector<SegmentIndex>& separators) const;

    /** output intersections to QString */
    QString to_string(const SegmentContainer& segments) const;
};

#endif // MMLT_INTERSECTION_INTERSECTIONS_H

