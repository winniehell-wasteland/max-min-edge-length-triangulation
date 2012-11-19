#include <QFileInfo>
#include <QTextStream>

#include <QxtJSON>

#include "json_parser.h"

JSONParser::JSONParser(QFile& file) :
    file_(file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        logger->error(msg("Could not open %1!").arg(file.fileName()));
        return;
    }
}

void JSONParser::parse(PointSet& point_set)
{

    QVariant points = QxtJSON::parse(QTextStream(&file_).readAll());

    if(!points.canConvert(QVariant::List))
    {
        logger->error(msg("Expected point list!"));
        return;
    }

    QVariant point;
    foreach(point, points.value<QVariantList>())
    {
        if(!point.canConvert(QVariant::List))
        {
            logger->error(msg("Expected point, got %1!").arg(point.toString()));
            continue;
        }

        QVariantList coordinates = point.value<QVariantList>();

        if(coordinates.size() == 2)
        {
            Point cgal_point(parse_number(coordinates[0]),
                             parse_number(coordinates[1]));

            logger->debug(msg("Created point (%1, %2)")
                          .arg(CGAL::to_double(cgal_point.x()))
                          .arg(CGAL::to_double(cgal_point.y())));

            point_set.insert(cgal_point);
        }
        else
        {
            logger->error(msg("Point has invalid dimension (%1)!").arg(coordinates.size()));
        }
    }
}

Number JSONParser::parse_number(const QVariant& value)
{
    if(value.canConvert(QVariant::Int))
    {
        return value.toInt();
    }
    else if(value.canConvert(QVariant::Double))
    {
        return value.toDouble();
    }
    else
    {
        logger->error(msg("Can not parse coordinate %1!").arg(value.toString()));
        return Number();
    }
}

