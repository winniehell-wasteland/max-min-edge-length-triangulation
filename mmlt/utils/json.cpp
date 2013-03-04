#include "json.h"

bool JSON::isArray(const JSON::JSONValue& value)
{
    return (value.type() == json_spirit::array_type);
}

bool JSON::isInt(const JSON::JSONValue& value)
{
    return (value.type() == json_spirit::int_type);
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

double JSON::toReal(const JSON::JSONValue& value)
{
    return value.get_real();
}

const std::string& JSON::toString(const JSON::JSONValue& value)
{
    return value.get_str();
}

Number JSON::parse_number(const JSON::JSONValue& value)
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
