#ifndef MMT_UTILS_H
#define MMT_UTILS_H

#include "config.h"

#include "logger.h"

#include "containers/segment_container.h"

extern Logger logger;

extern LineOrder       line_order;
extern PointOrder      point_order;
extern SegmentOrder    segment_order;

long to_int(const Number& number);

QString print_point(const Point& point);
QString print_segment(const Segment& segment);
QString print_igroup(const SegmentContainer& segments,
                     const IntersectionGroup& igroup);

#endif // MMT_UTILS_H
