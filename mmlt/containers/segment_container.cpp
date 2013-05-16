#include <vector>

#include "utils/assertions.h"
#include "utils/logger.h"

#include "segment_container.h"

/**
 * generate segment
 */
template <typename OutputIterator>
void generate_segments(const PointSet& points, OutputIterator output)
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
    // restrict scope for temporary vector
    {
        // temporary vector for increased insertion speed
        std::vector<Segment> tmp;

        generate_segments<std::back_insert_iterator<std::vector<Segment>>>(points, std::back_inserter(tmp));
        MMLT_postcondition(tmp.size() == points.size()*(points.size() - 1)/2);

        // add generated segments to set
        this->insert(tmp.begin(), tmp.end());
    }

    SegmentIndex seg_index = 0;

    // assign indices to the segments
    for(Segment& segment : *this)
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

void SegmentContainer::draw_range(QPainter& painter,
                                  const SegmentIndex& lower_bound,
                                  const SegmentIndex& upper_bound) const
{
    MMLT_assertion(upper_bound <= this->size());

    auto end = this->cbegin() + upper_bound;
    for(auto it = this->cbegin() + lower_bound; it != end; ++it)
    {
        it->draw(painter);
    }
}
