#ifndef MMT__STATS_H
#define MMT__STATS_H

#include <QString>

#include "cgal/kernel.h"

#include "utils/assertions.h"

namespace MMLT
{

class Stats
{
public:
    int          iteration;
    quint64      sat_solving_time;

    Stats() :
        iteration(0),
        sat_solving_time(0),
        lower_bound_(0),
        upper_bound_(std::numeric_limits<SegmentIndex>::max())
    {

    }

    void add_lower_bound(const SegmentIndex& bound)
    {
        if(bound > lower_bound_)
        {
            lower_bound_ = bound;
        }
    }

    void add_upper_bound(const SegmentIndex& bound)
    {
        if(bound < upper_bound_)
        {
            upper_bound_ = bound;
        }
    }

    SegmentIndex gap() const
    {
        MMLT_assertion(upper_bound_ >= lower_bound_);
        return (upper_bound_ - lower_bound_);
    }

    const SegmentIndex& lower_bound() const
    {
        return lower_bound_;
    }

    const SegmentIndex& upper_bound() const
    {
        return upper_bound_;
    }

    QString to_string() const
    {
        return QString("it=%1\tlb=%2\tub=%3")
                .arg(iteration)
                .arg(lower_bound_)
                .arg(upper_bound_);
    }
private:
    SegmentIndex lower_bound_;
    SegmentIndex upper_bound_;
};

}

#endif // MMT__STATS_H
