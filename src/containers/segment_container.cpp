#include "logger.h"
#include "mmt_utils.h"

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

SegmentContainer::SegmentContainer(const PointSet& points)
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
            CGAL_precondition_msg(point_order(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");

            push_back(Segment(*it, *jt, seg_index++));

            logger.debug(msg("Created segment %1").arg(print_segment(back())));
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
