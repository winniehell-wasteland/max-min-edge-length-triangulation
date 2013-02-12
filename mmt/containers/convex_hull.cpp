#include <iterator>

#include <CGAL/convex_hull_2.h>

#include "utils/logger.h"

#include "convex_hull.h"

ConvexHull::ConvexHull(const PointSet& points)
{
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(*this));
}

const SegmentIndex& ConvexHull::shortest_segment(const SegmentContainer& segments) const
{
    auto min_s = this->end()-1, min_t = this->begin();
    auto min_dist = CGAL::squared_distance(*min_s, *min_t);

    // iterate over all convex hull segments
    auto s = this->begin(), t = s;
    for(++t; t != this->end(); ++t)
    {
        auto dist = CGAL::squared_distance(*s, *t);

        if(dist < min_dist)
        {
            min_s = s;
            min_t = t;
            min_dist = dist;
        }
    }

    auto min_segment = std::lower_bound(segments.begin(), segments.end(), Segment(*min_s, *min_t), stl_segment_length_order);
    logger.info(mmt_msg("shortest convex hull segment: %1 (len^2=%2)")
                .arg(min_segment->to_string())
                .arg(CGAL::to_double(min_dist)));
    return min_segment->data().index;
}
