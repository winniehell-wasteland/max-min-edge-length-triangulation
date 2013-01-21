#ifndef MMT__CGAL__POINT_H
#define MMT__CGAL__POINT_H

#include <QString>

template <class Kernel_>
class PointC2 :
        public CGAL::PointC2<Kernel_>
{
    typedef typename Kernel_::FT    FT;
    typedef CGAL::PointC2<Kernel_>  PointBase;
public:

    PointC2()
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

    QString to_string() const
    {
        return QString("(%1,%2)")
                .arg(CGAL::to_double(this->x()))
                .arg(CGAL::to_double(this->y()));
    }
};

#endif // MMT__CGAL__POINT_H
