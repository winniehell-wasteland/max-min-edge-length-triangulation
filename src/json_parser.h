#ifndef MMT_JSON_PARSER_H
#define MMT_JSON_PARSER_H

#include <QFile>

#include "config.h"

//Q_DECLARE_METATYPE(Number);

class JSONParser
{
public:
    static const PointSet &parse(QFile& file);
    static Number parse_number(const QVariant& value);
};

#endif // MMT_JSON_PARSER_H
