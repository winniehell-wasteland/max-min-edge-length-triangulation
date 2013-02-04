#ifndef MMT__STATS_H
#define MMT__STATS_H

#include <QString>

#include "cgal/kernel.h"

struct Stats
{
    size_t       iteration;
    SegmentIndex lower_bound;
    SegmentIndex upper_bound;

    Stats() :
        iteration(0),
        lower_bound(),
        upper_bound()
    {

    }

    QString to_string() const
    {
        return QString("it=%1\tlb=%2\tub=%3")
                .arg(iteration)
                .arg(lower_bound)
                .arg(upper_bound);
    }
};

#endif // MMT__STATS_H
