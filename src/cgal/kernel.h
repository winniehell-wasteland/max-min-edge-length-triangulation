#ifndef MMT__CGAL__KERNEL_H
#define MMT__CGAL__KERNEL_H

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>

#include "cgal/point.h"
#include "cgal/segment.h"

typedef CGAL::Quotient<CGAL::MP_Float>  Number;

template < typename K_, typename Base_Kernel_ >
class Kernel_base :
        public Base_Kernel_::template Base<K_>::Type
{
    typedef K_      Kernel;
    //typedef typename Base_Kernel_::template Base<K_>::Type  BaseKernel;
    //typedef typename Base_Kernel_::RT                         RT;
    //typedef typename Base_Kernel_::FT                         FT;

    template < typename Kernel2 >
    struct Base { typedef Kernel_base<Kernel2, Base_Kernel_>  Type; };
public:
    typedef PointC2<Kernel>    Point_2;
    typedef SegmentC2<Kernel>  Segment_2;
};

struct Kernel
  : public CGAL::Type_equality_wrapper<
                Kernel_base<Kernel, CGAL::Cartesian<Number> >,
                Kernel >
{

};

typedef Kernel::Point_2          Point;
typedef Kernel::Segment_2        Segment;

#endif // MMT__CGAL__KERNEL_H
