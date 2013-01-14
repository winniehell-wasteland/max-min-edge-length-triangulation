#ifndef ORDERS_H
#define ORDERS_H

#include "config.h"

/** CGAL order for lines */
typedef Kernel::Compare_x_at_y_2           LineOrder;

/** CGAL order for points */
typedef Kernel::Compare_yx_2               PointOrder;
/** STL version of @ref PointOrder */
typedef CGAL::Compare_to_less<PointOrder>  STLPointOrder;

/**
 * @name global variables
 * @{
 */
extern const LineOrder        line_order;

extern const PointOrder       point_order;
extern const STLPointOrder    stl_point_order;
/**
 * @}
 */

#endif // ORDERS_H
