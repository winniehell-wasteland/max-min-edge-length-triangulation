#include "utils/assertions.h"
#include "utils/logger.h"

#include "segment_container.h"

SegmentContainer::SegmentContainer(const PointSet& points) :
    SegmentVector()
{
    // allocate space
    //segments_.reserve(points.size()*(points.size() - 1)/2);

    // generate segments
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        for(auto jt = it; ++jt != points.end(); )
        {
            MMT_precondition_msg(point_order(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");
            push_back(Segment(*it, *jt));
        }
    }

    // sort segments by length
    std::sort(this->begin(), this->end(), stl_segment_length_order);

    SegmentIndex seg_index = 0;

    for(auto segment = this->begin(); segment != this->end(); ++segment)
    {
        segment->data().index = seg_index;
        logger.debug(mmt_msg("Created segment %1").arg(segment->to_string()));

        ++seg_index;
    }

    CGAL_postcondition(seg_index == this->size());
}
