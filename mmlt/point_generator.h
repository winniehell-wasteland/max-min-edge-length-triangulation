#pragma once
#ifndef MMLT_POINT_GENERATOR_H
#define MMLT_POINT_GENERATOR_H

#include <set>

#include <CGAL/point_generators_2.h>

#include <QSettings>
#include <QString>

#include "cgal/kernel.h"
#include "cgal/orders.h"

#include "containers/point_set.h"

#include "utils/logger.h"
#include "utils/json.h"

class PointGenerator
{
public:
    using PointCreator = CGAL::Creator_uniform_2<int, Point>;

    static const double BASE_RANGE;

    static void run(const QSettings& settings)
    {
        if(!settings.childGroups().contains("generator"))
        {
            logger.error("config.json does not contain 'generator' section!");
            return;
        }

        QString type = settings.value("generator/type").toString();
        QString file_name = QString("%1_%2_%3_%4.json").arg(type);

        for(std::size_t num_points = 10; num_points <= 1000; num_points += 10)
        {
            if(type == "random_in_square")
            {
                generate<CGAL::Random_points_in_square_2<Point, PointCreator>>(file_name, num_points);
            }
        }
    }

private:
    template <typename GeneratorType>
    static void generate(const QString& file_name, std::size_t num_points)
    {
        double range = round(sqrt(BASE_RANGE*BASE_RANGE*num_points));
        GeneratorType generator(range);

        for(int i = 0; i < 10; ++i)
        {
            PointSet points;

            while(points.size() < num_points)
            {
                logger.debug(mmlt_msg("Generated %1 points").arg(points.size()));

                points.insert(*generator);
                ++generator;
            }

            logger.info(mmlt_msg("Generated %1 points").arg(points.size()));

            JSON::write_points(
                file_name
                .arg(num_points)
                .arg(range)
                .arg(i)
                .toStdString(),
                points
            );
        }
    }
};

const double PointGenerator::BASE_RANGE = 100.0;

#endif // MMLT_POINT_GENERATOR_H
