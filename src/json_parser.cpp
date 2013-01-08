#include <QDebug>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>

#include "json_parser.h"

PointSet JSONParser::parse(QFile& file)
{
  PointSet points(CGAL::compare_to_less(PointOrder()));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      qCritical() << msg("Could not open %1!").arg(file.fileName());
      return points;
    }

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

  if(!doc.isArray())
    {
      qCritical() << msg("Expected point list!");
      return points;
    }

  QJsonValue json_point;
  foreach(json_point, doc.array())
    {
      if(!json_point.isArray())
        {
          qCritical() << msg("Expected point, got %1!")
                         .arg(json_point.toString());
          continue;
        }

      QJsonArray coordinates = json_point.toArray(QJsonArray());

      if(coordinates.size() == 2)
        {
          Point point(parse_number(coordinates[0]),
              parse_number(coordinates[1]));

          qDebug() << msg("Created point (%1, %2)")
                      .arg(CGAL::to_double(point.x()))
                      .arg(CGAL::to_double(point.y()));

          points.insert(point);
        }
      else
        {
          qCritical() << msg("Point has invalid dimension (%1)!")
                         .arg(coordinates.size());
        }
    }

  qWarning() << msg("Read %1 points.").arg(points.size());

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
      qCritical() << msg("Can not parse coordinate %1!").arg(value.toString());
      return Number();
    }
}

