#pragma once
#ifndef MMLT_CGAL_KERNEL_H
#define MMLT_CGAL_KERNEL_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

//#include <CGAL/Cartesian.h>
//#include <CGAL/MP_Float.h>
//#include <CGAL/Quotient.h>

#include "cgal/point.h"
#include "cgal/segment.h"

/// number type
//using Number = CGAL::Quotient<CGAL::MP_Float>;

//using MMLT_kernel = CGAL::Cartesian<Number>;
using MMLT_kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

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
            Kernel_base<Kernel, MMLT_kernel>,
            Kernel
        >
{

};

/// for backwards compatibility
typedef Kernel::FT Number;

/// shortcut for point type
using Point = Kernel::Point_2;
/// shortcut for segment type
using Segment = Kernel::Segment_2;

#endif // MMLT_CGAL_KERNEL_H
