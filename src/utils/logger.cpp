#include <QDateTime>
#include <QTextStream>

static QTextStream qout(stdout);

#include "logger.h"

const Logger logger;
Messages mmt_msg;

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
