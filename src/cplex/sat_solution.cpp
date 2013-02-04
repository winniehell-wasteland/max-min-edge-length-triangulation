#include <QSvgGenerator>

#include "sat_solution.h"

void SATSolution::draw(QPainter& painter, const SegmentContainer& segments) const
{
    QPen pen(QColor(255, 0, 0));
    pen.setWidthF(1.0/MMT_SVG_SCALE);
    painter.setPen(pen);

    foreach(const SegmentIndex& segment_index, *this)
    {
        segments[segment_index].draw(painter);
    }
}

const SegmentIndex& SATSolution::value() const
{
    return *std::min_element(this->begin(), this->end());
}
