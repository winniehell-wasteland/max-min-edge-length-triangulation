#ifndef MMT_CONFIG_H
#define MMT_CONFIG_H

#include <map>
#include <set>
#include <vector>

#include <CGAL/assertions.h>
#include <CGAL/function_objects.h>

#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arr_segment_traits_2.h>

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>


typedef CGAL::Quotient<CGAL::MP_Float>  Number;
typedef CGAL::Cartesian<Number>         Kernel;

typedef CGAL::Point_2<Kernel>              Point;
typedef Kernel::Compare_yx_2               PointOrder;
typedef CGAL::Compare_to_less<PointOrder>  STLPointOrder;
typedef std::set<Point, STLPointOrder>     PointSet;

typedef CGAL::Arr_segment_traits_2<Kernel>  SegmentTraits;
typedef CGAL::Arr_segment_2<Kernel>         Segment;

class SegmentOrder
{
public:
  CGAL::Comparison_result operator() (const Segment& s, const Segment& t) const
  {
    CGAL_precondition_msg(point_order(s.source(), s.target()) == CGAL::SMALLER, "Segment points not in order!");
    CGAL_precondition_msg(point_order(t.source(), t.target()) == CGAL::SMALLER, "Segment points not in order!");

    switch(point_order(s.source(), t.source()))
      {
      case CGAL::SMALLER:
        return CGAL::SMALLER;
      case CGAL::LARGER:
        return CGAL::LARGER;
      default:
        switch(point_order(s.target(), t.target()))
          {
          case CGAL::SMALLER:
            return CGAL::SMALLER;
          case CGAL::LARGER:
            return CGAL::LARGER;
          default:
            return CGAL::EQUAL;
          };
      };
  }
private:
  PointOrder point_order;
};
typedef CGAL::Compare_to_less<SegmentOrder>  STLSegmentOrder;

/**
 * @name intersection defines
 * @{
 */
typedef std::set<Segment, STLSegmentOrder>  IntersectionGroup;
typedef IntersectionGroup::size_type        IntersectionGroupIndex;
typedef std::set<IntersectionGroupIndex>    IntersectionGroups;

typedef std::map<Segment, IntersectionGroups, STLSegmentOrder>  IntersectionMap;
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
