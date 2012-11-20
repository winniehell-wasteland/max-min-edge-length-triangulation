#ifndef MMT_SWEEP_LINE_VISITOR_H
#define MMT_SWEEP_LINE_VISITOR_H

#include "config.h"

#include <vector>

#include <CGAL/Sweep_line_2.h>
#include <CGAL/Sweep_line_empty_visitor.h>

#include <CGAL/Sweep_line_2/Sweep_line_2_utils.h>
#include <CGAL/Sweep_line_2/Sweep_line_2_visitors.h>

#include "utils.h"

/*! \class
 * A simple sweep-line visitor that determines if there are intersections
 * in the interiors of the given curve set.
 */
template <class Traits_>
class Sweep_line_visitor :
  public CGAL::Sweep_line_empty_visitor<Traits_>
{
  typedef Traits_                                      Traits_2;
  typedef Sweep_line_visitor<Traits_2>                 Self;

  typedef typename Traits_2::X_monotone_curve_2        X_monotone_curve_2;
  typedef typename Traits_2::Point_2                   Point_2;

  typedef CGAL::Sweep_line_empty_visitor<Traits_2>     Base;
  typedef typename Base::Event                         Event;
  typedef typename Base::Subcurve                      Subcurve;
  typedef typename Base::Status_line_iterator          Status_line_iterator;

  typedef CGAL::Sweep_line_2<Traits_2, Self>           Sweep_line_2;

protected:
  // Data members:
  size_t  m_found;           // How many intersections so far.

public:
  Sweep_line_visitor() :
    m_found(0)
  {}

  void add_subcurve (X_monotone_curve_2 curve,
                     Subcurve* subcurve)
  {
    logger->debug(msg("add subcurve: %1 and %2")
                  .arg(to_string(static_cast<const Segment&>(curve)))
                  .arg(subcurve_to_string(subcurve)));
  }

  template <class CurveIterator>
  void sweep(CurveIterator begin, CurveIterator end)
  {
    // Perform the sweep.
    Sweep_line_2* sl = reinterpret_cast<Sweep_line_2*>(this->sweep_line());

    sl->sweep(begin, end);
  }

  void update_event(Event* /*evt*/,
                    Subcurve* s,
                    Subcurve* t,
                    bool /* is_new */)
  {
    logger->debug(msg("intersecting segments: %1 and %2")
                  .arg(subcurve_to_string(s))
                  .arg(subcurve_to_string(t)));
    ++m_found;
  }

  void update_event(Event* /*evt*/,
                    Subcurve* s)
  {
    logger->debug(msg("hitting segment: %1")
                  .arg(subcurve_to_string(s)));

    ++m_found;
  }

  void update_event(Event* /* e */,
                    const Point_2& /* end_point */,
                    const X_monotone_curve_2& /* cv */,
                    CGAL::Arr_curve_end /* cv_end */,
                    bool /* is_new */)
  {}

  void update_event(Event* /* e */,
                    const X_monotone_curve_2& /* cv */,
                    CGAL::Arr_curve_end /* cv_end */,
                    bool /* is_new */)
  {}

  void update_event(Event* /* e */,
                    const Point_2& /* pt */,
                    bool /* is_new */)
  {}

  template <class XCurveIterator>
  void sweep_xcurves(XCurveIterator begin, XCurveIterator end)
  {
    // Perform the sweep.
    Sweep_line_2* sl = reinterpret_cast<Sweep_line_2*>(this->sweep_line());
    sl->sweep(begin, end);
  }

  void found_overlap(Subcurve* s,
                     Subcurve* t,
                     Subcurve* /* ov_sc */)
  {
    logger->debug(msg("overlapping segments: %1 and %2")
                  .arg(subcurve_to_string(s))
                  .arg(subcurve_to_string(t)));

    ++m_found;
  }

  bool after_handle_event(Event* /* event */,
                          Status_line_iterator /* iter */,
                          bool /* flag */)
  {
    logger->debug(msg("event done..."));
    return true;
  }

  int found_intersections() { return m_found; }

  QString subcurve_to_string(Subcurve* s)
  {
    return to_string(s->last_curve());
  }
};

#endif // MMT_SWEEP_LINE_VISITOR_H
