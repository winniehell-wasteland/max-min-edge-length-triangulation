#ifndef MMT__SEGMENT_H
#define MMT__SEGMENT_H

#include <QPainter>

#include <CGAL/Segment_2.h>

#include "containers/types.h"

#include "intersection/types.h"

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
 * line segment between two points
 * @see Point
 */
class Segment :
    public CGAL::Segment_2<Kernel>
{
  typedef CGAL::Segment_2<Kernel>  Base;
public:

  /** default constructor */
  Segment(const Point& source, const Point& target, SegmentIndex index) :
    Base(source, target),
    data_()
  {
    data().index = index;
  }

  /** copy constructor */
  Segment(const Segment& other) :
    Base(other.source(), other.target()),
    data_(other.data())
  {

  }

  /** assignment operator */
  Segment& operator=(const Segment& other)
  {
    Base::operator=(other);
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

  /** output segment using QPainter */
  void draw(QPainter& painter) const;

private:
  /** attached data */
  SegmentData data_;
};

/**
 * CGAL order for @ref Segment
 */
struct SegmentOrder
{
  CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const
  {
    //return CGAL::compare(s.data().index, t.data().index);
      if(s.data().index < t.data().index)
      {
          return CGAL::SMALLER;
      }
      else if(s.data().index < t.data().index)
      {
          return CGAL::LARGER;
      }
      else
      {
          return CGAL::EQUAL;
      }
  }
};

/**
 * STL version of @ref SegmentOrder
 */
typedef CGAL::Compare_to_less<SegmentOrder>  STLSegmentOrder;

/**
 * @name global variables
 * @{
 */
extern const SegmentOrder     segment_order;
extern const STLSegmentOrder  stl_segment_order;
/**
 * @}
 */

#endif // MMT__SEGMENT_H
