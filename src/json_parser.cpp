#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>

#include "json_parser.h"
#include "logger.h"

PointSet JSONParser::parse(QFile& file)
{
  PointSet points(CGAL::compare_to_less(PointOrder()));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      logger.error(mmt_msg("Could not open %1!").arg(file.fileName()));
      return points;
    }

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

  if(!doc.isArray())
    {
      logger.error(mmt_msg("Expected point list!"));
      return points;
    }

  QJsonValue json_point;
  foreach(json_point, doc.array())
    {
      if(!json_point.isArray())
        {
          logger.error(mmt_msg("Expected point, got %1!")
                       .arg(json_point.toString()));
          continue;
        }

      QJsonArray coordinates = json_point.toArray(QJsonArray());

      if(coordinates.size() == 2)
        {
          Point point(parse_number(coordinates[0]),
              parse_number(coordinates[1]));

          logger.debug(mmt_msg("Created point (%1, %2)")
                       .arg(CGAL::to_double(point.x()))
                       .arg(CGAL::to_double(point.y())));

          points.insert(point);
        }
      else
        {
          logger.error(mmt_msg("Point has invalid dimension (%1)!")
                       .arg(coordinates.size()));
        }
    }

  return points;
}

Number JSONParser::parse_number(const QJsonValue& value)
{
  if(value.isDouble())
    {
      return value.toDouble();
    }
  else
    {
      logger.error(mmt_msg("Can not parse coordinate %1!").arg(value.toString()));
      return Number();
    }
}

