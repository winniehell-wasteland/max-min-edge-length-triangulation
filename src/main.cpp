#include <QtCore>

#include <QxtLogger>

#include "config.h"
#include "controller.h"
#include "json_parser.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv) :
        QCoreApplication(argc, argv)
    {
    }

public slots:
    void processInputFiles()
    {
        QStringListIterator it(this->arguments());

        if(it.hasNext())
        {
            it.next();
        }

        while(it.hasNext())
        {
            QFile file(it.next());

            if(!file.exists())
            {
                logger->error(msg("File %1 does not exist!").arg(file.fileName()));
            }
            else
            {
                logger->info(msg("Reading file %1...").arg(file.fileName()));

                if(QFileInfo(file).suffix() != "json")
                {
                    logger->warning(tr("Only JSON files are supported!"));
                }

                JSONParser parser(file);

                PointSet points(CGAL::compare_to_less(PointOrder()));
                parser.parse(points);

                qxtLog->info(msg("Read %1 points.").arg(points.size()));

                Controller controller(points);

                qxtLog->info(msg("Starting controller..."));
                controller.start();
            }
        }

        emit quit();
    }
};

int main(int argc, char *argv[])
{
#ifdef NDEBUG
    qxtLog->enableLogLevels(QxtLogger::ErrorLevel);
#else
    qxtLog->enableLogLevels(QxtLogger::DebugLevel);
#endif

    Application application(argc, argv);

    QTimer::singleShot(0, &application, SLOT(processInputFiles()));
    
    return application.exec();
}

#include "main.moc"
