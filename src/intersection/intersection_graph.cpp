#include <CGAL/bounding_box.h>

#include <QPainter>
#include <QSvgGenerator>
#include <QTextStream>

#include "utils.h"

#include "arr_intersection_algorithm.h"
#include "naive_intersection_algorithm.h"
#include "sweep_intersection_algorithm.h"

#include "intersection_graph.h"

// typedef SweepIntersectionAlgorithm  IntersectionAlgorithm;
// typedef ArrIntersectionAlgorithm  IntersectionAlgorithm;
typedef NaiveIntersectionAlgorithm  IntersectionAlgorithm;

const double IntersectionGraph::SCALE = 4.0;

IntersectionGraph::IntersectionGraph(const PointSet& points) :
    IntersectionMap(CGAL::compare_to_less(SegmentOrder())),
    bounding_box_(CGAL::bounding_box(points.begin(), points.end()))
{
    for(PointSet::const_iterator it = points.begin(); it != points.end(); ++it)
    {
        PointSet::const_iterator jt = it;
        for(++jt; jt != points.end(); ++jt)
        {
            CGAL_precondition_msg(point_order_(*it, *jt) == CGAL::SMALLER, "PointSet is not ordered!");

            insert(std::make_pair(Segment(*it, *jt), Segments()));
        }
    }

    // find intersections
    IntersectionAlgorithm(this, points);
}

IntersectionGraph::mapped_type& IntersectionGraph::operator[] ( const IntersectionGraph::key_type& segment )
{
    CGAL_precondition_msg(point_order_(segment.source(), segment.target()) == CGAL::SMALLER, "Segment in wrong order!");

    return IntersectionMap::operator [](segment);
}

void IntersectionGraph::draw(const QString& fileName) const
{
  QSvgGenerator generator;
  generator.setFileName(fileName);

  generator.setSize(QSize(to_int(bounding_box_.xmax()) - to_int(bounding_box_.xmin()) + 20,
                          to_int(bounding_box_.ymax()) - to_int(bounding_box_.ymin()) + 20) * SCALE);

  generator.setViewBox(QRect(to_int(SCALE*(bounding_box_.xmin()-10)),
                             to_int(SCALE*(bounding_box_.ymin()-10)),
                             generator.size().width(),
                             generator.size().height()));

  QPainter painter;
  painter.begin(&generator);
  painter.scale(SCALE, SCALE);

  QPen pen(QColor(0, 0, 0));
  painter.setPen(pen);

  for(SegmentIterator segment = segments_begin(); segment != segments_end(); ++segment)
    {
      painter.drawLine(
            to_int(segment->source().x()),
            to_int(segment->source().y()),
            to_int(segment->target().x()),
            to_int(segment->target().y()));
    }

  //painter.fillRect(5, 5, 100, 100, QColor(255, 0, 0));
  painter.end();
}
