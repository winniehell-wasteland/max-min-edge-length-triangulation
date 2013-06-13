#include "utils/assertions.h"
#include "utils/logger.h"

#include "triangulation.h"

Triangulation::Triangulation(const PointSet& points)
{
    logger.info(mmlt_msg("Adding points to triangulation..."));
    this->insert(points.begin(), points.end());
}

void Triangulation::draw(QPainter& painter) const
{
    for(auto edge = this->finite_edges_begin();
        edge != this->finite_edges_end();
        ++edge)
    {
        this->segment(edge).draw(painter);
    }
}

const SegmentIndex& Triangulation::shortest_segment(const SegmentContainer& segments) const
{
    SegmentIndex min_index = segments.size();

    for(auto edge = this->finite_edges_begin(); edge != this->finite_edges_end(); ++edge)
    {
        const Segment& segment = this->segment(edge);

        auto it = segments.find(segment);
        logger.debug(mmlt_msg("Segment %1 is part of triangulation").arg(it->data().index));

        if(it->data().index < min_index)
        {
            min_index = it->data().index;
        }
    }

    return segments[min_index].data().index;

    /*
    for(const Segment& segment : segments)
    {
        Locate_type lt;
        int li;

        // locate first endpoint of segment
        Face_handle face = this->locate(segment.source(), lt, li);
        MMLT_postcondition(lt == VERTEX);
        Vertex_handle source = face->vertex(li);

        // locate second endpoint of segment
        face = this->locate(segment.target(), lt, li, face);
        MMLT_postcondition(lt == VERTEX);
        Vertex_handle target = face->vertex(li);

        // test if segment is part of triangulation
        if(this->is_edge(source, target))
        {
            logger.debug(mmlt_msg("Segment %1 is part of triangulation").arg(segment.data().index));
            return segment.data().index;
        }
    }

    // triangulation contains no segment
    // this should never happen
    MMLT_assertion(false);
    return segments[0].data().index;
    */


}
