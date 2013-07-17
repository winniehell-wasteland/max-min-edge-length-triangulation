#include "config.h"

#include <QtCore>
#include <QGuiApplication>

#include "controller.h"
#include "point_generator.h"

#include "utils/logger.h"

/**
 * flatten nested JSON objects to QSettings paths
 */
void parseJSONSetting(QVariantMap& map, const QString& prefix, const QJsonObject& setting)
{
    foreach(QString key, setting.keys())
    {
        QJsonValue value = setting.value(key);

        if(value.isObject())
        {
            parseJSONSetting(map, prefix + key + "/", value.toObject());
        }
        else
        {
            map.insert(prefix + key, value.toVariant());
        }
    }
}

bool readJSONSettings(QIODevice& device, QSettings::SettingsMap& map)
{
    if(device.atEnd())
    {
        logger.warn(mmlt_msg("No settings to read!"));
    }

    QJsonDocument doc = QJsonDocument::fromJson(device.readAll());

    if(doc.isEmpty())
    {
        logger.warn(mmlt_msg("Settings are empty!"));
    }
    else if(!doc.isObject())
    {
        logger.warn(mmlt_msg("Settings are not a JSON object!"));
    }
    else
    {
        parseJSONSetting(map, "", doc.object());
    }

    return true;
}

bool writeJSONSettings(QIODevice&, const QSettings::SettingsMap&)
{
    // not necessary
    return true;
}

const QSettings::Format JSONFormat =
        QSettings::registerFormat("json", readJSONSettings, writeJSONSettings);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        logger.error(mmlt_msg("Invalid number of arguments! Expected input file name."));
        return EXIT_FAILURE;
    }

    QStringList arguments;

    // read in arguments
    for(int i = 0; i < argc; ++i)
    {
        QString arg( argv[i] );
        arguments.append(arg);
    }

    logger.info( mmlt_msg("Running in directory %1").arg( QDir::currentPath() ) );

    QString settings_path( QDir::currentPath() + "/config.json" );
    logger.info( mmlt_msg( "Reading settings from %1" )
                 .arg( settings_path ) );
    const QSettings settings( settings_path, JSONFormat);

    if( arguments.at(1) == "--generate" )
    {
        PointGenerator::run(settings);
        return EXIT_SUCCESS;
    }
    else
    {
        QElapsedTimer total_time;
        total_time.start();

        QFile input_file( arguments.at(1) );
        QString file_prefix(
                    input_file
                    .fileName()
                    .left(
                        input_file
                        .fileName()
                        .lastIndexOf('.')
                        )
                    );

        Controller controller(file_prefix, input_file, settings);

        if( controller.start() )
        {
            logger.time( mmlt_msg( "controller initialization" ), total_time.elapsed());

            while( controller.iteration() )
            {

            }
        }
        else
        {
            logger.time( mmlt_msg( "controller initialization" ), total_time.elapsed());
        }

        controller.done();
        logger.time( mmlt_msg( "total" ), total_time.elapsed());

        return EXIT_SUCCESS;
    }
}
