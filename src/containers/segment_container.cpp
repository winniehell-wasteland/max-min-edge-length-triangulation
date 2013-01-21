#include "utils/assertions.h"
#include "utils/logger.h"

#include "segment_container.h"

class STLSegmentLengthComparator :
        public std::binary_function<Segment, Segment, bool>
{
public:
    STLSegmentLengthComparator(const SegmentContainer& segments) :
        segments_(segments)
    {

    }

    bool operator() (const SegmentIndex& s, const SegmentIndex& t) const
    {
        return segments_[s].squared_length() < segments_[t].squared_length();
    }

private:
    const SegmentContainer& segments_;
};

SegmentContainer::SegmentContainer(const PointSet& points) :
    SegmentVector()
{
    SegmentIndex seg_index = 0;

    // allocate space
    //segments_.reserve(points.size()*(points.size() - 1)/2);

    // generate segments
    for(PointSet::const_iterator it = points.begin(); it != points.end(); ++it)
    {
        PointSet::const_iterator jt;
        for(jt = it, ++jt; jt != points.end(); ++jt)
        {
            MMT_precondition_msg(point_order(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");

            Segment segment(*it, *jt);
            segment.data().index = seg_index++;
            logger.debug(mmt_msg("Created segment %1").arg(segment.to_string()));

            push_back(segment);
        }
    }

    CGAL_postcondition(seg_index == size());

    for(seg_index = 0; seg_index < size(); ++seg_index)
    {
        by_length_.push_back(seg_index);
    }

    // sort segments by length
    std::sort(by_length_.begin(), by_length_.end(), STLSegmentLengthComparator(*this));
}
