#pragma once
#ifndef MMLT_CGAL_KERNEL_H
#define MMLT_CGAL_KERNEL_H

#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>

#include "cgal/point.h"
#include "cgal/segment.h"

/// number type
typedef CGAL::Quotient<CGAL::MP_Float>  Number;

/**
 * kernel base with customized PointC2 and SegmentC2
 */
template < typename K_, typename Base_Kernel_ >
class Kernel_base :
        public Base_Kernel_::template Base<K_>::Type
{
    using Kernel = K_;

    template < typename Kernel2 >
    struct Base { using Type = Kernel_base<Kernel2, Base_Kernel_>; };
public:
    using Point_2 = PointC2<Kernel>;
    using Segment_2 = SegmentC2<Kernel>;
};

/**
 * customized kernel
 */
struct Kernel :
        public CGAL::Type_equality_wrapper<
            Kernel_base<Kernel, CGAL::Cartesian<Number>>,
            Kernel
        >
{

};

/// shortcut for point type
using Point = Kernel::Point_2;
/// shortcut for segment type
using Segment = Kernel::Segment_2;

#endif // MMLT_CGAL_KERNEL_H
