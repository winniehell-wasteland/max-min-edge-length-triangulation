#include <QSvgGenerator>

#include "utils/logger.h"

#include "sat_solution.h"

void SATSolution::draw_separators(QPainter& painter, const SegmentIndex& upper_bound, const SegmentContainer& segments) const
{
    for(const SegmentIndex& segment_index : *this)
    {
        if(upper_bound <= segment_index)
        {
            segments[segment_index].draw(painter);
        }
    }
}

void SATSolution::draw_short_segments(QPainter& painter, const SegmentIndex& upper_bound, const SegmentContainer& segments) const
{
    for(const SegmentIndex& segment_index : *this)
    {
        if(segment_index <upper_bound)
        {
            segments[segment_index].draw(painter);
        }
    }
}

const SegmentIndex& SATSolution::shortest_segment() const
{
    return *std::min_element(this->begin(), this->end());
}
