#ifndef MMT_INTERSECTION_GRAPH_H
#define MMT_INTERSECTION_GRAPH_H

#include "config.h"

#include <boost/iterator/transform_iterator.hpp>

#include <QString>

class IntersectionGraph :
        public IntersectionMap
{
  /** for drawing */
  static const double SCALE;
public:
    /**
     * helper functor to retrieve key from pair
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
    typedef boost::transform_iterator<key_functor, IntersectionGraph::const_iterator>  SegmentIterator;

    /** default constructor */
    IntersectionGraph(const PointSet& points);
    /** value access */
    mapped_type& operator[] ( const key_type& segment );

    /** output svg file */
    void draw(const QString &fileName) const;

    SegmentIterator segments_begin() const
    {
      return boost::make_transform_iterator(begin(), key_functor());
    }

    SegmentIterator segments_end() const
    {
      return boost::make_transform_iterator(end(), key_functor());
    }
private:
    const CGAL::Iso_rectangle_2<Kernel> bounding_box_;
    PointOrder point_order_;
};

#endif // MMT_INTERSECTION_GRAPH_H
