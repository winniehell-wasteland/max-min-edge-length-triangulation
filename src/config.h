#ifndef MMT_CONFIG_H
#define MMT_CONFIG_H

#define CGAL_USE_LONG_LONG

#include <CGAL/assertions.h>
#include <CGAL/function_objects.h>

#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
typedef CGAL::Quotient<CGAL::MP_Float>  Number;

#include <CGAL/Cartesian.h>
typedef CGAL::Cartesian<Number>         Kernel;

#include <CGAL/Point_2.h>
typedef CGAL::Point_2<Kernel>           Point;

const double MMT_SVG_SCALE = 4.0;

#endif // MMT_CONFIG_H
