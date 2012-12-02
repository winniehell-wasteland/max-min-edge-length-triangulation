#ifndef MMT_UTILS_H
#define MMT_UTILS_H

#include "config.h"

extern LineOrder       line_order;
extern PointOrder      point_order;
extern SegmentOrder    segment_order;

long to_int(const Number& number);

QString to_string(const Segment& segment);

#endif // MMT_UTILS_H
