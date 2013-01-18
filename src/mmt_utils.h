#ifndef MMT_UTILS_H
#define MMT_UTILS_H

#include "config.h"

#include "segment.h"

#include "containers/segment_container.h"

QString print_point(const Point& point);
QString print_segment(const Segment& segment);
QString print_igroup(const SegmentContainer& segments,
                     const IntersectionGroup& igroup);

#endif // MMT_UTILS_H
