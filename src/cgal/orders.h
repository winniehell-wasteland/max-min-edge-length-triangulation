#ifndef MMT__CGAL__ORDERS_H
#define MMT__CGAL__ORDERS_H

#include "cgal/kernel.h"

/** CGAL order for points */
typedef Kernel::Compare_yx_2               PointOrder;
/** STL version of @ref PointOrder */
typedef CGAL::Compare_to_less<PointOrder>  STLPointOrder;

/** CGAL order for @ref Segment by index */
struct SegmentIndexOrder
{
    CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const;
};

/** CGAL order for @ref Segment by length */
struct SegmentLengthOrder
{
    CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const;
};

/** STL version of @ref SegmentOrder */
typedef CGAL::Compare_to_less<SegmentIndexOrder>   STLSegmentIndexOrder;
/** STL version of @ref SegmentOrder */
typedef CGAL::Compare_to_less<SegmentLengthOrder>  STLSegmentLengthOrder;

/**
 * @name global variables
 * @{
 */
extern const PointOrder     point_order;
extern const STLPointOrder  stl_point_order;

extern const SegmentIndexOrder      segment_index_order;
extern const SegmentLengthOrder     segment_length_order;
extern const STLSegmentIndexOrder   stl_segment_index_order;
extern const STLSegmentLengthOrder  stl_segment_length_order;
/**
 * @}
 */

#endif // MMT__CGAL__ORDERS_H
