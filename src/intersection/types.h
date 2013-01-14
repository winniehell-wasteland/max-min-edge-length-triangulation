#ifndef INTERSECTION_TYPES_H
#define INTERSECTION_TYPES_H

#include <set>
#include <vector>

#include "index_types.h"

#include "containers/sorted_vector.h"

/** sorted container of intersecting segments */
typedef std::set<SegmentIndex>                IntersectionGroup;

/** vector of \ref IntersectionGroup */
typedef std::vector<IntersectionGroup>        IntersectionGroupVector;

/** sorted container of intersection group indices */
typedef SortedVector<IntersectionGroupIndex>  IntersectionGroupIndices;

#endif // INTERSECTION_TYPES_H
