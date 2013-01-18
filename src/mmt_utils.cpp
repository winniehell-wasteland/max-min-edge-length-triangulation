#include "mmt_utils.h"

QString print_point(const Point& point)
{
    return QString("(%2,%3)")
            .arg(CGAL::to_double(point.x()))
            .arg(CGAL::to_double(point.y()));
}

QString print_segment(const Segment& segment)
{
    return QString("%1:%2->%3")
            .arg(segment.data().index)
            .arg(print_point(segment.source()))
            .arg(print_point(segment.target()));
}

QString print_igroup(const SegmentContainer& segments,
                     const IntersectionGroup& igroup)
{
    QString result;

    foreach(SegmentIndex i, igroup)
    {
        result += print_segment(segments[i]) + ", ";
    }

    result.chop(2);

    return result;
}
