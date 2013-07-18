#pragma once
#ifndef MMLT_CGAL_POINT_H
#define MMLT_CGAL_POINT_H

#include <CGAL/Cartesian/Point_2.h>

#include <QPainter>
#include <QString>

namespace MMLT
{

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

    /** empty constructor */
    PointC2() :
        PointBase()
    {

    }

    /** origin constructor */
    PointC2(const CGAL::Origin& origin) :
        PointBase(origin)
    {

    }

    /** Cartesian constructor */
    PointC2(const FT& x, const FT& y) :
        PointBase(x, y)
    {

    }

    /** homogeneous constructor */
    PointC2(const FT &hx, const FT &hy, const FT &hw) :
        PointBase(hx, hy, hw)
    {

    }

    /** draw segment using given QPainter */
    void draw(QPainter& painter) const;
    /** dump point to QString */
    QString to_string() const;
};

template <class Kernel_>
void PointC2<Kernel_>::draw(QPainter& painter) const
{
    painter.drawPoint(CGAL::to_double(this->x()), CGAL::to_double(this->y()));
}

template <class Kernel_>
QString PointC2<Kernel_>::to_string() const
{
    return QString("(%1,%2)")
            .arg(CGAL::to_double(this->x()), 0, 'f', 0)
            .arg(CGAL::to_double(this->y()), 0, 'f', 0);
}

}

#endif // MMT__CGAL__POINT_H
