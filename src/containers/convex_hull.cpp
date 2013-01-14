#include <iterator>

#include <CGAL/convex_hull_2.h>

#include "convex_hull.h"

ConvexHull::ConvexHull(const PointSet& points)
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(*this));
}
