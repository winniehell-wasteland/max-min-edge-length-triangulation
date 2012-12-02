#ifndef MMT_CONFIG_H
#define MMT_CONFIG_H

#include <map>
#include <set>
#include <vector>

#include <CGAL/assertions.h>
#include <CGAL/function_objects.h>

#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arr_segment_traits_2.h>

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>


typedef CGAL::Quotient<CGAL::MP_Float>  Number;
typedef CGAL::Cartesian<Number>         Kernel;

/**
 * @name order definitions
 * @{
 */
typedef Kernel::Compare_yx_2               PointOrder;
typedef Kernel::Compare_x_at_y_2           LineOrder;
/**
 *@}
 */

/**
 * @name point definitions
 * @{
 */
typedef CGAL::Point_2<Kernel>              Point;
typedef CGAL::Compare_to_less<PointOrder>  STLPointOrder;
typedef std::set<Point, STLPointOrder>     PointSet;
/**
 * @}
 */

/**
 * @name index definitions
 * @{
 */
typedef size_t  IntersectionGroupIndex;
typedef size_t  SegmentIndex;

/** vector of intersection group indices */
typedef std::vector<IntersectionGroupIndex>    IntersectionGroupIndices;

typedef std::vector<SegmentIndex>              SegmentIndexVector;
/**
 * @}
 */

/**
 * @name segment definitions
 * @{
 */

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

class Segment :
    public CGAL::Segment_2<Kernel>
{
  typedef CGAL::Segment_2<Kernel>  Base;
public:

  /** default constructor */
  Segment(const Point& source, const Point& target) :
    Base(source, target),
    data_()
  {

  }

  Segment(const Segment& other) :
    Base(other.source(), other.target()),
    data_(other.data())
  {

  }

  Segment& operator=(const Segment& other)
  {
    Base::operator=(other);
    data_ = other.data();

    return *this;
  }

  SegmentData& data()
  {
    return data_;
  }

  const SegmentData& data() const
  {
    return data_;
  }

private:
  SegmentData data_;
};

struct SegmentOrder
{
  CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const
  {
    return CGAL::compare(s.data().index, t.data().index);
  }
};

typedef CGAL::Compare_to_less<SegmentOrder>  STLSegmentOrder;
typedef std::vector<Segment>                 SegmentVector;

/**
 * @}
 */

/**
 * @name intersection defines
 * @{
 */
typedef std::set<SegmentIndex>  IntersectionGroup;
/**
 * @}
 */

// logger
#include <QxtLogger>
#define logger qxtLog

// helper class for string literals
class Messages : QObject
{
  Q_OBJECT
};
#define msg Messages::tr

#endif // MMT_CONFIG_H
