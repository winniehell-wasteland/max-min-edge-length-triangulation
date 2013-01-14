#include "segment.h"

const SegmentOrder segment_order        = SegmentOrder();
const STLSegmentOrder stl_segment_order = CGAL::compare_to_less(segment_order);
