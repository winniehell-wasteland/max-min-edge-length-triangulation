#ifndef MMT__INTERSECTION__INTERSECTION_GROUP_H
#define MMT__INTERSECTION__INTERSECTION_GROUP_H

#include <set>
#include <vector>

#include <QPainter>

#include "containers/segment_container.h"

/** sorted container of intersecting segments */
typedef std::set<SegmentIndex>  SegmentIndexSet;

class IntersectionGroup :
        public SegmentIndexSet
{
public:
    IntersectionGroup() :
        SegmentIndexSet()
    {

    }

    void draw(QPainter& painter, const SegmentContainer& segments) const;
    QString to_string(const SegmentContainer& segments) const;
};

/** vector of \ref IntersectionGroup */
typedef std::vector<IntersectionGroup>        IntersectionGroupVector;

#endif // MMT__INTERSECTION__INTERSECTION_GROUP_H
