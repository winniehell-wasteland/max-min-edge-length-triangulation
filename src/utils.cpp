#include "utils.h"

long to_int(const Number& number)
{
  double d = CGAL::to_double(number);

  if(d > 0.0)
    {
      return std::floor(d + 0.5);
    }
  else
    {
      return std::ceil(d - 0.5);
    }
}

QString to_string(const Segment& segment)
{
  return QString("(%1,%2)->(%3,%4)")
      .arg(CGAL::to_double(segment.source().x()))
      .arg(CGAL::to_double(segment.source().y()))
      .arg(CGAL::to_double(segment.target().x()))
      .arg(CGAL::to_double(segment.target().y()));
}
