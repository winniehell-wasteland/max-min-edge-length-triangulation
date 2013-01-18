#include "segment.h"

const SegmentOrder segment_order        = SegmentOrder();
const STLSegmentOrder stl_segment_order = CGAL::compare_to_less(segment_order);

void Segment::draw(QPainter &painter) const
{
    painter.drawLine(
                CGAL::to_double(source().x()),
                CGAL::to_double(source().y()),
                CGAL::to_double(target().x()),
                CGAL::to_double(target().y()));
}
