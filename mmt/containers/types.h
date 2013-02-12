#ifndef MMT__CONTAINERS__TYPES_H
#define MMT__CONTAINERS__TYPES_H

#include "config.h"

#include <set>
#include <vector>

#include "cgal/orders.h"
#include "containers/sorted_vector.h"

/**
 * @name point containers
 * @{
 */

/** (sorted) set of points */
typedef std::set<Point, STLPointOrder>       PointSet;

/** (unsorted) vector of points */
typedef std::vector<Point>                   PointVector;

/**
 * @}
 */

#endif // MMT__CONTAINERS__TYPES_H
