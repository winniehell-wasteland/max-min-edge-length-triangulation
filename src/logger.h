#ifndef MMT_LOGGER_H
#define MMT_LOGGER_H

#include <QString>

class Logger
{
public:
    Logger();

    void debug(const QString& message) const;
    void info(const QString& message) const;
    void warn(const QString& message) const;
    void error(const QString& message) const;
    void print(const QString& message) const;
};

extern const Logger  logger;

#endif // MMT_LOGGER_H
