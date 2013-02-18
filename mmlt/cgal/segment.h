#pragma once
#ifndef MMLT_CGAL_SEGMENT_H
#define MMLT_CGAL_SEGMENT_H

#include "config.h"

#include <boost/container/flat_set.hpp>

#include <CGAL/Cartesian/Segment_2.h>

#include <QPainter>
#include <QString>

/** sorted container of intersection group indices */
using IntersectionGroupIndices = boost::container::flat_set<IntersectionGroupIndex>;

/**
 * data attached to a segment
 */
struct SegmentData
{
  SegmentIndex              index;
  IntersectionGroupIndices  intersection_groups;
  /** true if the segment overlaps another */
  bool                      overlapping;
};

/**
 * customized segment type
 */
template <class Kernel_>
class SegmentC2 :
        public CGAL::SegmentC2<Kernel_>
{
    using SegmentBase = CGAL::SegmentC2<Kernel_>;

    using Point_2   = typename Kernel_::Point_2;
    using Segment_2 = typename Kernel_::Segment_2;
public:
    /** empty constructor */
    SegmentC2() :
        SegmentBase()
    {

    }

    /** base constructor */
    SegmentC2(const Point_2& source, const Point_2& target) :
        SegmentBase(source, target)
    {

    }

    /** copy constructor */
    SegmentC2(const SegmentC2& other) :
      SegmentBase(other),
      data_(other.data())
    {

    }

    /** assignment operator */
    SegmentC2& operator=(const SegmentC2& other)
    {
      SegmentBase::operator=(other);
      data_ = other.data();

      return *this;
    }

    /** getter for attached data */
    SegmentData& data()
    {
      return data_;
    }

    /** constant getter for attached data */
    const SegmentData& data() const
    {
      return data_;
    }

    /// draw segment using given QPainter
    void draw(QPainter& painter) const;
    /// dump segment to QString
    QString to_string() const;
private:
    SegmentData  data_;
};

template <class Kernel_>
void SegmentC2<Kernel_>::draw(QPainter& painter) const
{
    painter.drawLine(
                CGAL::to_double(this->source().x()),
                CGAL::to_double(this->source().y()),
                CGAL::to_double(this->target().x()),
                CGAL::to_double(this->target().y())
    );
}

template <class Kernel_>
QString SegmentC2<Kernel_>::to_string() const
{
    return QString("%1:%2->%3")
            .arg(this->data().index)
            .arg(this->source().to_string())
            .arg(this->target().to_string()
    );
}

#endif // MMLT_CGAL_SEGMENT_H
