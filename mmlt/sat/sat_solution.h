#pragma once
#ifndef MMLT_SAT_SAT_SOLUTION_H
#define MMLT_SAT_SAT_SOLUTION_H

#include "config.h"

#include <vector>

#include <QPainter>

#include "containers/segment_container.h"

class SATSolution :
        public std::vector<SegmentIndex>
{
public:
    SATSolution() :
        std::vector<SegmentIndex>()
    {

    }

    void draw_short_segments(QPainter& painter,
                             const SegmentIndex& num_short_segments,
                             const SegmentContainer& segments) const;

    void draw_separators(QPainter& painter,
                         const SegmentIndex& num_short_segments,
                         const SegmentContainer& segments) const;

    const SegmentIndex& shortest_segment() const;
};

#endif // MMLT_SAT_SAT_SOLUTION_H
