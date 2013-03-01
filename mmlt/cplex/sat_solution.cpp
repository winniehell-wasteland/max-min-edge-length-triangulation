#include <QSvgGenerator>

#include "utils/logger.h"

#include "sat_solution.h"

void SATSolution::draw(QPainter& painter, const SegmentContainer& segments) const
{
    for(const SegmentIndex& segment_index : *this)
    {
        segments[segment_index].draw(painter);
    }
}

const SegmentIndex& SATSolution::shortest_segment() const
{
    return *std::min_element(this->begin(), this->end());
}
