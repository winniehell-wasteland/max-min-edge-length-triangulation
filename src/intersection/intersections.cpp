#include "intersections.h"

namespace MMLT
{

void Intersections::draw(QPainter& painter, const SegmentContainer& segments) const
{
    for(const SegmentIndex& i : *this)
    {
        segments[i].draw(painter);
    }
}

void Intersections::find_separators(const SegmentIndex& segment_index,
                                    const SegmentContainer& segments,
                                    std::vector<SegmentIndex>& separators) const
{
    const Segment& segment = segments[segment_index];

    for(const SegmentIndex& separator_index : *this)
    {
        // segments with lower index are shorter (or same length)
        if(segment_index < separator_index)
        {
            const Segment& separator = segments[separator_index];

            // check that we got no outer segment
            // and no segment of same size
            if(!separator.data().is_outer
                    && (segment.squared_length() < separator.squared_length()))
            {
                // indices come in sorted
                separators.push_back(separator_index);
            }
        }
    }
}

QString Intersections::to_string(const SegmentContainer& segments) const
{
    QString result;

    for(const SegmentIndex& i : *this)
    {
        result += segments[i].to_string() + ", ";
    }

    result.chop(2);

    return result;
}

}
