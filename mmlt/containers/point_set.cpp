#include <QFileInfo>

#include "utils/json_parser.h"
#include "utils/logger.h"

#include "point_set.h"

PointSet::PointSet(QFile &input_file) :
    std::set<Point, STLPointOrder>(stl_point_order)
{
    if(QFileInfo(input_file).suffix() != "json")
    {
        logger.warn(mmlt_msg("Only JSON files are supported!"));
    }

    if(!JSONParser::parse_points(input_file, std::inserter(*this, this->end())))
    {
        this->clear();
    }
}

void PointSet::draw(QPainter& painter) const
{
    for(const Point& point : *this)
    {
        point.draw(painter);
    }
}
