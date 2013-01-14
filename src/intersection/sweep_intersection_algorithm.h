#ifndef MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H

#include "config.h"

#include <algorithm>
#include <deque>

#include "intersection_graph.h"
#include "cgal_utils.h"

/*
#include <CGAL/Sweep_line_2.h>

#include "intersection_graph.h"
#include "sweep_line_visitor.h"
*/

class SweepIntersectionAlgorithm
{

  /*
  typedef Sweep_line_visitor<SegmentTraits>           Visitor;
  typedef CGAL::Sweep_line_2<SegmentTraits, Visitor>  Sweep_line;
  */
public:
  SweepIntersectionAlgorithm(IntersectionGraph& intersection_graph,
                             const PointSet& points) :
    intersection_graph_(intersection_graph),
    overlaps_(),
    points_(points),
    status_order_(intersection_graph.segments()),
    status_structure_(status_order_)
  {
    /*
    Visitor     visitor;
    Sweep_line  sweep_line(&visitor);
    visitor.sweep(graph.segments().begin(), graph.segments().end());

    logger->info(msg("Intersections: %1").arg(visitor.found_intersections()));
    */


  }

private:
  class StatusOrder
  {
  public:
    StatusOrder(const SegmentVector& segments) :
      segments_(segments),
      current_event_(CGAL::ORIGIN)
    {

    }

    void set_current_event(const Point& event)
    {
      current_event_ = event;
    }

    bool operator() (const SegmentIndex& si, const SegmentIndex& ti) const
    {
      const CGAL::Segment_2<Kernel>& s = segments_[si], t = segments_[ti];

      // both segments intersect in current event
      if(s.has_on(current_event_) && (s.source() != current_event_) &&
         t.has_on(current_event_) && (t.source() != current_event_))
        {
          return false;
        }

      if(s.is_horizontal())
        {
          if(t.is_horizontal())
            {
              CGAL_precondition(CGAL::SMALLER == point_order(s.source(),
                                                             s.target()));
              CGAL_precondition(CGAL::SMALLER == point_order(t.source(),
                                                             t.target()));

              // compare first point of segment
              return (CGAL::SMALLER == point_order(s.source(), t.source()));
            }
          else
            {
              // compare first point of segment with projection
              return (CGAL::SMALLER == line_order(s.source(),
                                                  t.supporting_line()));
            }
        }
      else if(t.is_horizontal())
        {
          // compare first point of segment with projection
          return (CGAL::LARGER == line_order(t.source(),
                                             s.supporting_line()));
        }
      else
        {
          // compare projections
          return (CGAL::SMALLER == line_order(current_event_,
                                              s.supporting_line(),
                                              t.supporting_line()));
        }
    }

    bool operator() (const Point& p, const SegmentIndex& si) const
    {
      const CGAL::Segment_2<Kernel>& s = segments_[si];
      return (s.is_horizontal() && (s.target() == p)) ||
          CGAL::left_turn(s.source(), s.target(), p);
    }

    bool operator() (const SegmentIndex& si, const Point& p) const
    {
      const CGAL::Segment_2<Kernel>& s = segments_[si];
      return (s.is_horizontal() && (s.target() == p)) ||
          CGAL::right_turn(s.source(), s.target(), p);
    }

  private:
    const SegmentVector& segments_;
    Point current_event_;
  };

  typedef std::multiset<SegmentIndex, StatusOrder>  StatusStructure;
  typedef std::pair<StatusStructure::iterator,
                    StatusStructure::iterator>      SegmentRange;

  IntersectionGraph&   intersection_graph_;
  SegmentIndices   overlaps_;
  const PointSet&      points_;
  StatusOrder          status_order_;
  StatusStructure      status_structure_;

  void assert_valid_status() const
  {
    for(StatusStructure::const_iterator s = status_structure_.begin();
        s != status_structure_.end(); ++s )
      {
        StatusStructure::const_iterator next = s;
        ++next;

        CGAL_assertion(!status_order_(*next, *s));
      }
  }

  void handle_event_point(const Point& event)
  {
    PointSet::const_iterator event_it = points_.find(event);
    bool is_input_point = (event_it != points_.end());

    SegmentIndices incident_segments;
    move_incident_segments(event, incident_segments);

    // generate intersection group only if there are intersecting segments
    bool generate_igroup = false;

    for(SegmentIndices::const_iterator si = incident_segments.begin();
        si != incident_segments.end(); ++si)
      {
        const CGAL::Segment_2<Kernel> s = intersection_graph_.segments()[*si];

        // event point lies in interior of segment
        if(s.target() != event)
          {
            CGAL_precondition(s.source() != event);

            // segments must not contain input points
            if(is_input_point)
              {
                overlaps_.push_back(*si);
              }
            else
              {
                status_structure_.insert(*si);
                generate_igroup = true;
              }
          }
      }

    assert_valid_status();

    if(generate_igroup)
      {
        IntersectionGroup igroup(incident_segments.begin(),
                                 incident_segments.end());

        intersection_graph_.add_intersection_group(igroup);
      }

    // insert segments with event point as upper (left) endpoint
    if(is_input_point)
      {
        PointSet::const_iterator target = event_it;
        ++target;

        if(target != points_.end())
          {
            SegmentVector::const_iterator segment =
                intersection_graph_.find_segment(SegmentBase(event, *target));

            for(; segment != intersection_graph_.segments().end(); ++segment)
              {
                if(segment->source() != event)
                  {
                    break;
                  }

                status_structure_.insert(segment->data().index);
              }
          }
      }

    assert_valid_status();
  }

  void move_incident_segments(const Point& point,
                              SegmentIndices& incident_segments)
  {
    SegmentRange range = std::equal_range(status_structure_.begin(),
                                          status_structure_.end(),
                                          point, status_order_);

    incident_segments.assign(range.first, range.second);
    status_structure_.erase(incident_segments.first, incident_segments.second);
  }
};

#endif // MMT_INTERSECTION_SWEEP_INTERSECTION_ALGORITHM_H
