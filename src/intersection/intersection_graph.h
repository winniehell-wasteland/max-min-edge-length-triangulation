#ifndef MMT_INTERSECTION_GRAPH_H
#define MMT_INTERSECTION_GRAPH_H

#include "config.h"

#include <boost/iterator/transform_iterator.hpp>

#include <QString>
#include <QSvgGenerator>

#include "parameters.h"

class IntersectionGraph :
        public IntersectionMap
{
  /** for drawing */
  static const double SCALE;

  typedef std::vector<IntersectionGroup>  IntersectionGroupVector;
public:
    /**
     * helper functor to retrieve key from map value pair
     */
    struct key_functor {
        template<class> struct result;

        template<class F, class T>
        struct result<F(T)> {
            typedef key_type type;
        };

        const key_type& operator ()(const value_type& value) const {
            return value.first;
        }
    };

    /** iterator over map keys */
    typedef boost::transform_iterator<key_functor,
                                      const_iterator>  SegmentIterator;

    /** default constructor */
    IntersectionGraph(const PointSet& points);
    /** value access */
    mapped_type& operator[] ( const key_type& segment );

    void add_intersection_group(const IntersectionGroup& group);

    /** output intersection graph to SVG */
    void draw(const QString &prefix, const Parameters &parameters) const;

    void remove_overlap(const Segment& segment);

    SegmentIterator segments_begin() const
    {
      return boost::make_transform_iterator(begin(), key_functor());
    }

    SegmentIterator segments_end() const
    {
      return boost::make_transform_iterator(end(), key_functor());
    }
private:
    const CGAL::Iso_rectangle_2<Kernel>  bounding_box_;
    IntersectionGroupVector              intersection_groups_;
    PointOrder                           point_order_;

    void init_generator(QSvgGenerator& generator) const;
    void draw_graph(QPainter &painter) const;
    void draw_intersection_group(QPainter& painter,
                                 const IntersectionGroup& group) const;
};

#endif // MMT_INTERSECTION_GRAPH_H
