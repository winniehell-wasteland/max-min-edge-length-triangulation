#ifndef MMT_JSON_PARSER_H
#define MMT_JSON_PARSER_H

#include <QFile>

#include "config.h"

//Q_DECLARE_METATYPE(Number);

class JSONParser
{
public:
    JSONParser(QFile &file);
    void parse(PointSet &point_set);
private:
    QFile& file_;

    Number parse_number(const QVariant& value);
};

#endif // MMT_JSON_PARSER_H
