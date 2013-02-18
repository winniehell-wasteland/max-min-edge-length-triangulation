#include <QJsonArray>
#include <QJsonDocument>

#include "utils/assertions.h"
#include "utils/logger.h"

#include "json_parser.h"

Number parse_number(const QJsonValue& value)
{
    if(value.isDouble())
    {
        return value.toDouble();
    }
    else
    {
        logger.error(mmlt_msg("Cannot parse coordinate %1!").arg(value.toString()));
        return Number();
    }
}

bool JSONParser::parse_points(QFile& file, std::insert_iterator<PointSet> output)
{
    if(!file.exists())
    {
        logger.error(mmlt_msg("File %1 does not exist!").arg(file.fileName()));
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        logger.error(mmlt_msg("Could not open %1!").arg(file.fileName()));
        return false;
    }

    logger.info(mmlt_msg("Reading file %1...").arg(file.fileName()));

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    if(!doc.isArray())
    {
        logger.error(mmlt_msg("Expected point list!"));
        return false;
    }

    // loop over JSON array
    for(const QJsonValue& json_point : doc.array())
    {
        MMLT_precondition_msg(
            json_point.isArray(),
            mmlt_msg("Expected point, got %1!")
            .arg(json_point.toString())
        );

        QJsonArray coordinates = json_point.toArray(QJsonArray());

        MMLT_precondition_msg(
            (coordinates.size() == 2),
            mmlt_msg("Point has invalid dimension (%1)!")
            .arg(coordinates.size())
        );

        Point point(parse_number(coordinates[0]),
                parse_number(coordinates[1]));

        logger.debug(mmlt_msg("Created point (%1, %2)")
                     .arg(CGAL::to_double(point.x()))
                     .arg(CGAL::to_double(point.y())));

        *output = point;
        ++output;
    }

    return true;
}
