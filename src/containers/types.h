#ifndef CONTAINERS__TYPES_H
#define CONTAINERS__TYPES_H

#include "config.h"

#include <set>
#include <vector>

#include "index_types.h"
#include "orders.h"

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

/**
 * @name segment containers
 * @{
 */

/** sorted container of segment indices */
typedef SortedVector<SegmentIndex>           SegmentIndices;

/**
 * @}
 */

#endif // CONTAINERS__TYPES_H
