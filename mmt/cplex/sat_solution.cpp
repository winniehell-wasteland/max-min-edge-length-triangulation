#include <QSvgGenerator>

#include "sat_solution.h"

void SATSolution::draw(QPainter& painter, const SegmentContainer& segments) const
{
    foreach(const SegmentIndex& segment_index, *this)
    {
        segments[segment_index].draw(painter);
    }
}

const SegmentIndex& SATSolution::shortest_segment() const
{
    return *std::min_element(this->begin(), this->end());
}
