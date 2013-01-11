#ifndef MMT_INTERSECTION_GRAPH_H
#define MMT_INTERSECTION_GRAPH_H

#include "config.h"

#include <QString>
#include <QSvgGenerator>

#include "parameters.h"

#include "containers/segment_container.h"

class IntersectionGraph
{
  /** for drawing */
  static const double SCALE;

  typedef std::vector<IntersectionGroup>  IntersectionGroupVector;
public:

    /** default constructor */
    IntersectionGraph(const PointSet& points, SegmentContainer& segments);

    void add_intersection_group(const IntersectionGroup& group);

    /** output intersection graph to SVG */
    void draw_igraph(const QString &prefix) const;

    /** output intersection groups to SVG */
    void draw_igroups(const QString &prefix) const;
private:
    const CGAL::Iso_rectangle_2<Kernel>  bounding_box_;
    IntersectionGroupVector              intersection_groups_;
    SegmentContainer&                    segments_;

    void init_generator(QSvgGenerator& generator) const;
    void draw_graph(QPainter &painter) const;
    void draw_intersection_group(QPainter& painter,
                                 const IntersectionGroup& group) const;
};

#endif // MMT_INTERSECTION_GRAPH_H
