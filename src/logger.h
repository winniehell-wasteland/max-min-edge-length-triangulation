#ifndef MMT_LOGGER_H
#define MMT_LOGGER_H

#include <QObject>
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

/** helper class for string literals */
class Messages : QObject
{
  Q_OBJECT
public:
  QString operator()(const char* text)
  {
      return this->tr(text);
  }
};

extern const Logger  logger;
extern Messages mmt_msg;

#endif // MMT_LOGGER_H
