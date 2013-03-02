#include <vector>

#include <boost/foreach.hpp>

#include "utils/assertions.h"
#include "utils/logger.h"

#include "segment_container.h"

void generate_segments(const PointSet& points, std::back_insert_iterator<std::vector<Segment>> output)
{
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        for(auto jt = it; ++jt != points.end(); )
        {
            MMLT_precondition_msg(point_order(*it, *jt) == CGAL::SMALLER, "Point set is not ordered!");
            *output = Segment(*it, *jt);
            ++output;
        }
    }
}

SegmentContainer::SegmentContainer(const PointSet& points) :
    boost::container::flat_multiset<Segment, STLSegmentLengthOrder>(stl_segment_length_order)
{
    std::vector<Segment> tmp;

    generate_segments(points, std::back_inserter(tmp));
    MMLT_postcondition(tmp.size() == points.size()*(points.size() - 1)/2);

    this->insert(tmp.begin(), tmp.end());

    SegmentIndex seg_index = 0;

    BOOST_FOREACH(Segment& segment, *this)
    {
        segment.data().index = seg_index;
        logger.debug(mmlt_msg("Created segment %1").arg(segment.to_string()));

        ++seg_index;
    }
    MMLT_postcondition(seg_index == this->size());
}

Segment& SegmentContainer::operator[](const SegmentIndex& index)
{
    return *(this->begin() + index);
}

const Segment& SegmentContainer::operator[](const SegmentIndex& index) const
{
    return *(this->cbegin() + index);
}

void SegmentContainer::draw(QPainter& painter) const
{
    for(const Segment& segment : *this)
    {
        segment.draw(painter);
    }
}
