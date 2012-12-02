#ifndef MMT_INTERSECTION_GRAPH_H
#define MMT_INTERSECTION_GRAPH_H

#include "config.h"

#include <QString>
#include <QSvgGenerator>

#include "parameters.h"

class IntersectionGraph
{
  /** for drawing */
  static const double SCALE;

  typedef std::vector<IntersectionGroup>  IntersectionGroupVector;
public:

    /** default constructor */
    IntersectionGraph(const PointSet& points);

    void add_intersection_group(const IntersectionGroup& group);

    /** output intersection graph to SVG */
    void draw(const QString &prefix, const Parameters &parameters) const;

    SegmentVector& segments()
    {
      return segments_;
    }

    const SegmentVector& segments() const
    {
      return segments_;
    }
private:
    const CGAL::Iso_rectangle_2<Kernel>  bounding_box_;
    IntersectionGroupVector              intersection_groups_;
    SegmentVector                        segments_;

    void init_generator(QSvgGenerator& generator) const;
    void draw_graph(QPainter &painter) const;
    void draw_intersection_group(QPainter& painter,
                                 const IntersectionGroup& group) const;
};

#endif // MMT_INTERSECTION_GRAPH_H
