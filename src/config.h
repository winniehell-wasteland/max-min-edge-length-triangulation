#ifndef MMT_CONFIG_H
#define MMT_CONFIG_H

#define CGAL_USE_LONG_LONG
/*
#include <map>
#include <set>
#include <vector>
*/
#include <CGAL/assertions.h>
#include <CGAL/function_objects.h>

/*
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arr_segment_traits_2.h>
*/

#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
typedef CGAL::Quotient<CGAL::MP_Float>  Number;

#include <CGAL/Cartesian.h>
typedef CGAL::Cartesian<Number>         Kernel;

#include <CGAL/Point_2.h>
typedef CGAL::Point_2<Kernel>           Point;

#include <QObject>
/** helper class for string literals */
class Messages : QObject
{
  Q_OBJECT
};
#define msg Messages::tr

#endif // MMT_CONFIG_H
