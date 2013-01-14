#ifndef CONTAINERS__CONVEX_HULL_H
#define CONTAINERS__CONVEX_HULL_H

#include "containers/types.h"

class ConvexHull :
        public PointVector
{
public:
    ConvexHull(const PointSet& points);
private:
    typedef PointVector  base;
};

#endif // CONTAINERS__CONVEX_HULL_H
