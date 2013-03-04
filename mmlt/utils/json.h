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
#include <json_spirit/json_spirit_writer_template.h>
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
    static JSONValue fromNumber(const Number& value);
    static JSONArray fromPoint(const Point& point);

    static bool isArray(const JSONValue& value);
    static bool isInt(const JSONValue& value);
    static bool isNumber(const JSONValue& value);
    static bool isReal(const JSONValue& value);

    static const JSONArray& toArray(const JSONValue& value);
    static int toInt(const JSONValue& value);
    static Number toNumber(const JSONValue& value);
    static Point toPoint(const JSONValue& value);
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

        std::ifstream input(file.fileName().toStdString(), std::ifstream::in);

        if(!input.good())
        {
            logger.error(mmlt_msg("Could not open %1!").arg(file.fileName()));
            return false;
        }

        logger.info(mmlt_msg("Reading file %1...").arg(file.fileName()));

        JSONValue doc;
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
        for(const JSONValue& value : toArray(doc))
        {
            Point point = toPoint(value);

            logger.debug(mmlt_msg("Created point %1")
                         .arg(point.to_string()));

            *output = point;
            ++output;
        }

        return true;
    }

    template <typename Container>
    static bool write_points(const std::string& file_name, Container points)
    {
        std::ofstream output(file_name, std::ifstream::out);

        if(!output.good())
        {
            logger.error(mmlt_msg("Could not open %1!").arg(QString::fromStdString(file_name)));
            return false;
        }

        logger.info(mmlt_msg("Writing file %1...").arg(QString::fromStdString(file_name)));

        JSONArray doc;
        for(const Point& point : points)
        {
            doc.push_back(fromPoint(point));
        }

        json_spirit::write_stream(JSONValue(doc), output, false);

        return true;
    }
};

#endif // MMLT_UTILS_JSON_H
