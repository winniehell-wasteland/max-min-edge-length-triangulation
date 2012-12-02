#include <QFileInfo>
#include <QTextStream>

#include <QxtJSON>

#include "json_parser.h"

PointSet JSONParser::parse(QFile& file)
{
  PointSet points(CGAL::compare_to_less(PointOrder()));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      logger->error(msg("Could not open %1!").arg(file.fileName()));
      return points;
    }

  QVariant json_points = QxtJSON::parse(QTextStream(&file).readAll());

  if(!json_points.canConvert(QVariant::List))
    {
      logger->error(msg("Expected point list!"));
      return points;
    }

  QVariant json_point;
  foreach(json_point, json_points.value<QVariantList>())
    {
      if(!json_point.canConvert(QVariant::List))
        {
          logger->error(msg("Expected point, got %1!")
                        .arg(json_point.toString()));
          continue;
        }

      QVariantList coordinates = json_point.value<QVariantList>();

      if(coordinates.size() == 2)
        {
          Point point(parse_number(coordinates[0]),
              parse_number(coordinates[1]));

          logger->debug(msg("Created point (%1, %2)")
                        .arg(CGAL::to_double(point.x()))
                        .arg(CGAL::to_double(point.y())));

          points.insert(point);
        }
      else
        {
          logger->error(msg("Point has invalid dimension (%1)!")
                        .arg(coordinates.size()));
        }
    }

  qxtLog->info(msg("Read %1 points.").arg(points.size()));

  return points;
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

