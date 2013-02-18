#include "orders.h"

CGAL::Comparison_result SegmentIndexOrder::operator() (const Segment& s, const Segment& t) const
{
    const SegmentIndex& si = s.data().index,
            ti = t.data().index;

    if(si < ti)
    {
        return CGAL::SMALLER;
    }
    else if(si > ti)
    {
        return CGAL::LARGER;
    }
    else
    {
        return CGAL::EQUAL;
    }
}

CGAL::Comparison_result SegmentLengthOrder::operator() (const Segment& s, const Segment& t) const
{
    Number sl = s.squared_length(),
            tl = t.squared_length();

    if(sl < tl)
    {
        return CGAL::SMALLER;
    }
    else if(sl > tl)
    {
        return CGAL::LARGER;
    }
    else
    {
        return CGAL::EQUAL;
    }
}

const PointOrder point_order            = PointOrder();
const STLPointOrder stl_point_order     = CGAL::compare_to_less(point_order);

const SegmentIndexOrder segment_index_order          = SegmentIndexOrder();
const SegmentLengthOrder segment_length_order        = SegmentLengthOrder();
const STLSegmentIndexOrder stl_segment_index_order   = CGAL::compare_to_less(segment_index_order);
const STLSegmentLengthOrder stl_segment_length_order = CGAL::compare_to_less(segment_length_order);
