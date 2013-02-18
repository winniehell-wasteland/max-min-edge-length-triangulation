#pragma once
#ifndef MMLT_CGAL_POINT_H
#define MMLT_CGAL_POINT_H

#include <CGAL/Cartesian/Point_2.h>

#include <QString>

/**
 * customized point type
 */
template <class Kernel_>
class PointC2 :
        public CGAL::PointC2<Kernel_>
{
    typedef typename Kernel_::FT    FT;
    typedef CGAL::PointC2<Kernel_>  PointBase;
public:

    PointC2() :
        PointBase()
    {

    }

    PointC2(const CGAL::Origin& origin) :
        PointBase(origin)
    {

    }

    PointC2(const FT& x, const FT& y) :
        PointBase(x, y)
    {

    }

    PointC2(const FT &hx, const FT &hy, const FT &hw) :
        PointBase(hx, hy, hw)
    {

    }

    /// dump point to QString
    QString to_string() const;
};

template <class Kernel_>
QString PointC2<Kernel_>::to_string() const
{
    return QString("(%1,%2)")
            .arg(CGAL::to_double(this->x()))
            .arg(CGAL::to_double(this->y()));
}

#endif // MMT__CGAL__POINT_H
