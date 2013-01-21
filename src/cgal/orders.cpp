#include "orders.h"

const LineOrder line_order              = LineOrder();

const PointOrder point_order            = PointOrder();
const STLPointOrder stl_point_order     = CGAL::compare_to_less(point_order);

const SegmentOrder segment_order        = SegmentOrder();
const STLSegmentOrder stl_segment_order = CGAL::compare_to_less(segment_order);
