#include "intersection_group.h"

void IntersectionGroup::draw(QPainter& painter, const SegmentContainer& segments) const
{
    foreach(const SegmentIndex& i, *this)
    {
        segments[i].draw(painter);
    }
}

QString IntersectionGroup::to_string(const SegmentContainer& segments) const
{
    QString result;

    foreach(SegmentIndex i, *this)
    {
        result += segments[i].to_string() + ", ";
    }

    result.chop(2);

    return result;
}
