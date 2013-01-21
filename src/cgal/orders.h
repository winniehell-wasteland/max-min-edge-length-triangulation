#ifndef MMT__CGAL__ORDERS_H
#define MMT__CGAL__ORDERS_H

#include "cgal/kernel.h"

/** CGAL order for lines */
typedef Kernel::Compare_x_at_y_2           LineOrder;

/** CGAL order for points */
typedef Kernel::Compare_yx_2               PointOrder;
/** STL version of @ref PointOrder */
typedef CGAL::Compare_to_less<PointOrder>  STLPointOrder;

/**
 * CGAL order for @ref Segment
 */
struct SegmentOrder
{
  CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const
  {
    //return CGAL::compare(s.data().index, t.data().index);
      if(s.data().index < t.data().index)
      {
          return CGAL::SMALLER;
      }
      else if(s.data().index < t.data().index)
      {
          return CGAL::LARGER;
      }
      else
      {
          return CGAL::EQUAL;
      }
  }
};

/**
 * STL version of @ref SegmentOrder
 */
typedef CGAL::Compare_to_less<SegmentOrder>  STLSegmentOrder;

/**
 * @name global variables
 * @{
 */
extern const LineOrder        line_order;

extern const PointOrder       point_order;
extern const STLPointOrder    stl_point_order;

extern const SegmentOrder     segment_order;
extern const STLSegmentOrder  stl_segment_order;
/**
 * @}
 */

#endif // MMT__CGAL__ORDERS_H
