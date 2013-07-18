#pragma once
#ifndef MMLT_UTILS_LOGGER_H
#define MMLT_UTILS_LOGGER_H

#include <boost/format.hpp>

#include <QObject>
#include <QString>

#include "cgal/kernel.h"
#include "stats.h"

namespace MMLT
{

class Logger
{
public:
    static const boost::format fmt_log;

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

/** dump segment length to QString */
QString segment_length_to_string(const Segment& segment);

}

#endif // MMLT_UTILS_LOGGER_H
