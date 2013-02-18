#pragma once
#ifndef MMLT_UTILS_JSON_PARSER_H
#define MMLT_UTILS_JSON_PARSER_H

#include <QFile>

#include "cgal/kernel.h"
#include "containers/point_set.h"

class JSONParser
{
public:
    static bool parse_points(QFile& file, std::insert_iterator<PointSet> output);
};

#endif // MMLT_UTILS_JSON_PARSER_H
