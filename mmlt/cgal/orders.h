#pragma once
#ifndef MMLT_CGAL_ORDERS_H
#define MMLT_CGAL_ORDERS_H

#include "cgal/kernel.h"

/** CGAL order for points */
using PointOrder = Kernel::Compare_yx_2;
/** STL version of @ref PointOrder */
using STLPointOrder = CGAL::Compare_to_less<PointOrder>;

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
using STLSegmentIndexOrder = CGAL::Compare_to_less<SegmentIndexOrder>;
/** STL version of @ref SegmentOrder */
using STLSegmentLengthOrder = CGAL::Compare_to_less<SegmentLengthOrder>;

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

#endif // MMLT_CGAL_ORDERS_H
