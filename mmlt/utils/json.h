#pragma once
#ifndef MMLT_UTILS_JSON_H
#define MMLT_UTILS_JSON_H

#include <fstream>

#include <QFile>

#include "cgal/kernel.h"

#include "containers/point_set.h"

#include "utils/assertions.h"
#include "utils/logger.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <json_spirit/json_spirit_reader_template.h>
#pragma GCC diagnostic pop

class JSON
{
public:
    /**
     * wrapper for JSON array implementation
     */
    using JSONArray = json_spirit::Array;

    /**
     * wrapper for JSON value implementation
     */
    using JSONValue = json_spirit::Value;

    /**
     * @name helper functions
     * @{
     */
    static bool isArray(const JSONValue& value);
    static bool isInt(const JSONValue& value);

    static bool isNumber(const JSONValue& value)
    {
        return (isInt(value) || isReal(value));
    }

    static bool isReal(const JSONValue& value);

    static const JSONArray& toArray(const JSONValue& value);
    static int toInt(const JSONValue& value);
    static double toReal(const JSONValue& value);
    static const std::string& toString(const JSONValue& value);
    /**
     * @}
     */

    template <typename OutputIterator>
    static bool read_points(QFile& file, OutputIterator output)
    {
        if(!file.exists())
        {
            logger.error(mmlt_msg("File %1 does not exist!").arg(file.fileName()));
            return false;
        }

        JSONValue doc;

        std::ifstream input(file.fileName().toStdString(), std::ifstream::in);

        if(!input.good())
        {
            logger.error(mmlt_msg("Could not open %1!").arg(file.fileName()));
            return false;
        }

        logger.info(mmlt_msg("Reading file %1...").arg(file.fileName()));
        if(!json_spirit::read_stream(input, doc))
        {
            return false;
        }

        if(!isArray(doc))
        {
            logger.error(mmlt_msg("Expected point list!"));
            return false;
        }

        // loop over JSON array
        for(const JSONValue& json_point : toArray(doc))
        {
            MMLT_precondition_msg(
                isArray(json_point),
                mmlt_msg("Expected point, got %1!")
                .arg(QString::fromStdString(toString(json_point)))
            );

            const JSONArray& coordinates = toArray(json_point);

            MMLT_precondition_msg(
                (coordinates.size() == 2),
                mmlt_msg("Point has invalid dimension (%1)!")
                .arg(coordinates.size())
            );

            Point point(parse_number(coordinates[0]),
                    parse_number(coordinates[1]));

            logger.debug(mmlt_msg("Created point %1")
                         .arg(point.to_string()));

            *output = point;
            ++output;
        }

        return true;
    }

private:
    static Number parse_number(const JSONValue& value);
};

#endif // MMLT_UTILS_JSON_H
