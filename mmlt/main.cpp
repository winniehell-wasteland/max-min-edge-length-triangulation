#include "config.h"

#include <QtCore>
#include <QGuiApplication>

#include "controller.h"

#include "utils/logger.h"

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

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        logger.error(mmlt_msg("Invalid number of arguments!"));
        return EXIT_FAILURE;
    }

    logger.info(mmlt_msg("Running in directory %1").arg(QDir::currentPath()));

    QGuiApplication application(argc, argv);
    const QSettings settings( QDir::currentPath() + "/config.json", JSONFormat);

    Controller controller(application, settings);
    QTimer::singleShot(0, &controller, SLOT(start()));

    return application.exec();
}
