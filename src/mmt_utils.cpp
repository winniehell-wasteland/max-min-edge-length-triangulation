#include "mmt_utils.h"

Logger logger;

LineOrder line_order = LineOrder();
PointOrder point_order = CGAL::compare_to_less(PointOrder());
SegmentOrder segment_order = CGAL::compare_to_less(SegmentOrder());

long to_int(const Number& number)
{
  double d = CGAL::to_double(number);

  if(d > 0.0)
    {
      return std::floor(d + 0.5);
    }
  else
    {
      return std::ceil(d - 0.5);
    }
}

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
