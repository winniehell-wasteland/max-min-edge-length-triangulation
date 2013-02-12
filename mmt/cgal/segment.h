#ifndef MMT__CGAL__SEGMENT_H
#define MMT__CGAL__SEGMENT_H

#include <QPainter>
#include <QString>

#include "config.h"
#include "containers/sorted_vector.h"

/** sorted container of intersection group indices */
typedef SortedVector<IntersectionGroupIndex>  IntersectionGroupIndices;

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

template <class Kernel_>
class SegmentC2 :
        public CGAL::SegmentC2<Kernel_>
{
    typedef CGAL::SegmentC2<Kernel_>               SegmentBase;

    typedef typename Kernel_::Point_2              Point_2;
    typedef typename Kernel_::Segment_2            Segment_2;
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

    void draw(QPainter& painter) const
    {
        painter.drawLine(
                    CGAL::to_double(this->source().x()),
                    CGAL::to_double(this->source().y()),
                    CGAL::to_double(this->target().x()),
                    CGAL::to_double(this->target().y()));
    }


    QString to_string() const
    {
        return QString("%1:%2->%3")
                .arg(this->data().index)
                .arg(this->source().to_string())
                .arg(this->target().to_string());
    }
private:
    SegmentData  data_;
};

#endif // MMT__CGAL__SEGMENT_H
