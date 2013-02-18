#include "utils/assertions.h"
#include "utils/logger.h"

#include "triangulation.h"

SegmentIndex DelaunayTriangulation::shortest_segment(const SegmentContainer& segments) const
{
    Face_handle face;
    Locate_type lt;
    int li;
    Vertex_handle s, t;

    // iterate sorted segments
    for(const Segment& segment : segments)
    {
        // locate segment source
        face = this->locate(segment.source(), lt, li);
        MMLT_precondition(lt == VERTEX);
        s = face->vertex(li);

        // locate segment target
        face = this->locate(segment.target(), lt, li, face);
        MMLT_precondition(lt == VERTEX);
        t = face->vertex(li);

        if(this->is_edge(s, t))
        {
            logger.info(mmlt_msg("shortest Delaunay segment: %1 (len^2=%2)")
                        .arg(segment.to_string())
                        .arg(CGAL::to_double(segment.squared_length())));

            return segment.data().index;
        }
    }

    return segments.size();
}
