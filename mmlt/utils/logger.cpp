#include <QDateTime>
#include <QTextStream>

//static const boost::format fmt_log();

static QTextStream qout(stdout);

#include "logger.h"

const Logger logger;
Messages mmlt_msg;

Logger::Logger()
{
}

#ifndef NDEBUG
void Logger::debug(const QString& message) const
{
    print("[DEBUG] "+message);
}
#else
void Logger::debug(const QString&) const
{

}
#endif

void Logger::info(const QString& message) const
{
    print("[INFO] "+message);
}

void Logger::warn(const QString& message) const
{
    print("[WARN] "+message);
}

void Logger::error(const QString& message) const
{
    print("[ERROR] "+message);
}

void Logger::print(const QString& message) const
{
    qout << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ")
         << message
         << endl;
}

void Logger::stats(const Stats& stats) const
{
    print("[STATS] "+stats.to_string());
}

void Logger::time(const QString& identifier, int milliseconds) const
{
    print(QString("[TIME] %1=%2ms").arg(identifier).arg(milliseconds));
}

QString segment_length_to_string(const Segment& segment)
{
    auto length = segment.squared_length();

    QString s;
    QTextStream stream(&s);

    stream << "len^2=";
    stream << CGAL::to_double(length);

    return s;
}
