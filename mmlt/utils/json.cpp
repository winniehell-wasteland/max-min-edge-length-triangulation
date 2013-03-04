#include "json.h"

JSON::JSONValue JSON::fromNumber(const Number& value)
{
    return JSONValue(CGAL::to_double(value));
}

JSON::JSONArray JSON::fromPoint(const Point& point)
{
    JSONArray array;

    array.push_back(fromNumber(point.x()));
    array.push_back(fromNumber(point.y()));

    return array;
}

bool JSON::isArray(const JSON::JSONValue& value)
{
    return (value.type() == json_spirit::array_type);
}

bool JSON::isInt(const JSON::JSONValue& value)
{
    return (value.type() == json_spirit::int_type);
}

bool JSON::isNumber(const JSONValue& value)
{
    return (isInt(value) || isReal(value));
}

bool JSON::isReal(const JSON::JSONValue& value)
{
    return (value.type() == json_spirit::real_type);
}

const JSON::JSONArray& JSON::toArray(const JSON::JSONValue& value)
{
    return value.get_array();
}

int JSON::toInt(const JSON::JSONValue& value)
{
    return value.get_int();
}

Number JSON::toNumber(const JSON::JSONValue& value)
{
    MMLT_precondition_msg(
        isNumber(value),
        "Coordinate is not a number!"
    );

    if(isInt(value))
    {
        return toInt(value);
    }
    else
    {
        MMLT_precondition(isReal(value));
        return toReal(value);
    }
}

Point JSON::toPoint(const JSONValue& value)
{
    MMLT_precondition_msg(
        isArray(value),
        mmlt_msg("Expected point, got %1!")
        .arg(QString::fromStdString(toString(value)))
    );

    const JSONArray& coordinates = toArray(value);

    MMLT_precondition_msg(
        (coordinates.size() == 2),
        mmlt_msg("Point has invalid dimension (%1)!")
        .arg(coordinates.size())
    );

    return Point(
        toNumber(coordinates[0]),
        toNumber(coordinates[1])
    );
}

double JSON::toReal(const JSON::JSONValue& value)
{
    return value.get_real();
}

const std::string& JSON::toString(const JSON::JSONValue& value)
{
    return value.get_str();
}
