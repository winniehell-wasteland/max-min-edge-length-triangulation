#pragma once
#ifndef MMLT_GENERATOR_H
#define MMLT_GENERATOR_H

#include <set>

#include <CGAL/point_generators_2.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include "cgal/kernel.h"
#include "cgal/orders.h"

#include "utils/logger.h"

class Generator
{
public:
    using Creator = CGAL::Creator_uniform_2<int, Point>;

    Generator()
    {
        CGAL::Random_points_in_square_2<Point, Creator> g(250.0);
        std::set<Point, STLPointOrder> points(stl_point_order);

        while(points.size() < 500)
        {
            points.insert(*g);
            ++g;
        }

        logger.info(mmlt_msg("Generated %1 points").arg(points.size()));

        QJsonArray point_arr;
        for(const Point& point : points)
        {
            QJsonArray coordinates;

            coordinates.append(int(CGAL::to_double(point.x())));
            coordinates.append(int(CGAL::to_double(point.y())));

            point_arr.append(coordinates);
        }

        QFile file(QString("random_%1_0000_0500_01.json").arg("500"));
        file.open(QFile::ReadWrite);
        file.write(QJsonDocument(point_arr).toJson());
        file.close();
    }
};

#endif // MMLT_GENERATOR_H
