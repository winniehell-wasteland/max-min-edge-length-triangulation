#ifndef MMT_JSON_PARSER_H
#define MMT_JSON_PARSER_H

#include <QFile>

#include "config.h"

//Q_DECLARE_METATYPE(Number);

class JSONParser
{
public:
    static PointSet parse(QFile& file);
    static Number parse_number(const QJsonValue &value);
};

#endif // MMT_JSON_PARSER_H
