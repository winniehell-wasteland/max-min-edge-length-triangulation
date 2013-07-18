#pragma once
#ifndef MMLT_POINT_GENERATOR_H
#define MMLT_POINT_GENERATOR_H

#include <set>

#include <CGAL/point_generators_2.h>

#include <QString>

#include "cgal/kernel.h"
#include "cgal/orders.h"

#include "containers/point_set.h"

#include "utils/logger.h"
#include "utils/json.h"
#include "utils/settings.h"

namespace MMLT
{

class PointGenerator
{
public:
    using PointCreator = CGAL::Creator_uniform_2<int, Point>;

    static const double BASE_RANGE;

    static void run(const Settings& settings)
    {
        QString base_name = QString("%1_%2_%3_%4.json")
                .arg( QString::fromStdString( settings.generator_type ) );

        std::size_t num_iterations = settings.generator_iterations;

        for(std::size_t num_points = 10; num_points <= 1000; num_points += 10)
        {
            logger.info(mmlt_msg("#points = %1").arg(num_points));

            if(settings.generator_type == "random_in_square")
            {
                generate<CGAL::Random_points_in_square_2<Point, PointCreator>>(base_name, num_points, num_iterations);
            }
        }
    }

private:
    template <typename GeneratorType>
    static void generate(const QString& base_name, std::size_t num_points, std::size_t num_iterations)
    {
        double range = round(sqrt(BASE_RANGE*BASE_RANGE*num_points));
        GeneratorType generator(range);

        for(std::size_t i = 0; i < num_iterations; ++i)
        {
            QString file_name = base_name
                .arg(quint64(num_points), 4, 10, QChar('0'))
                .arg(quint64(range),      4, 10, QChar('0'))
                .arg(quint64(i),          3, 10, QChar('0'));

            if(QFile(file_name).exists())
            {
                continue;
            }

            PointSet points;

            while(points.size() < num_points)
            {
                logger.debug(mmlt_msg("Generated %1 points").arg(points.size()));

                points.insert(*generator);
                ++generator;
            }

            logger.debug(mmlt_msg("Generated %1 points").arg(points.size()));

            JSON::write_points(file_name.toStdString(), points);
        }
    }
};

const double PointGenerator::BASE_RANGE = 100.0;

}

#endif // MMLT_POINT_GENERATOR_H
