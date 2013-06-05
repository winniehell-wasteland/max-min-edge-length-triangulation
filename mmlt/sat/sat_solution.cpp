#include "utils/assertions.h"
#include "utils/logger.h"

#include "sat_solution.h"

void SATSolution::draw_separators(QPainter& painter, const SegmentIndex& num_short_segments, const SegmentContainer& segments) const
{
    for(const SegmentIndex& segment_index : *this)
    {
        if(num_short_segments <= segment_index)
        {
            segments[segment_index].draw(painter);
        }
    }
}

void SATSolution::draw_short_segments(QPainter& painter, const SegmentIndex& num_short_segments, const SegmentContainer& segments) const
{
    for(const SegmentIndex& segment_index : *this)
    {
        if(segment_index < num_short_segments)
        {
            segments[segment_index].draw(painter);
        }
    }
}

const SegmentIndex& SATSolution::shortest_segment() const
{
    MMLT_precondition(!this->empty());
    return *std::min_element(this->begin(), this->end());
}
