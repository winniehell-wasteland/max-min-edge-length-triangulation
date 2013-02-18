#pragma once
#ifndef MMLT_UTILS_LOGGER_H
#define MMLT_UTILS_LOGGER_H

#include <QObject>
#include <QString>

#include "stats.h"

class Logger
{
public:
    Logger();

    void debug(const QString& message) const;
    void info(const QString& message) const;
    void warn(const QString& message) const;
    void error(const QString& message) const;
    void print(const QString& message) const;
    void stats(const Stats& stats) const;
    void time(const QString& identifier, int milliseconds) const;
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
extern Messages      mmlt_msg;

#endif // MMLT_UTILS_LOGGER_H
