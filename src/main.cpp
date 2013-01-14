#include "config.h"

#include <QtCore>

#include "controller.h"
#include "logger.h"
#include "parameters.h"

class Application : public QCoreApplication
{
  Q_OBJECT

public:
  Application(int &argc, char **argv) :
    QCoreApplication(argc, argv),
    parameters_()
  {
  }

public slots:
  void processArguments()
  {
    QStringListIterator it(this->arguments());

    // ignore execution path
    if(it.hasNext())
      {
        it.next();
      }

    while(it.hasNext())
      {
        QString argument = it.next();

        if(argument.startsWith("--"))
          {
            if(argument == "--draw-igraph")
              {
                parameters_.draw_igraph = true;
              }
            else if(argument == "--draw-igroups")
              {
                parameters_.draw_igroups = true;
              }
            else
              {
                logger.error(msg("Unknown argument: %1").arg(argument));
              }
          }
        else
          {
            QFile file(argument);

            if(!file.exists())
              {
                logger.error(msg("File %1 does not exist!").arg(argument));
              }
            else
              {
                logger.info(msg("Reading file %1...").arg(file.fileName()));

                if(QFileInfo(file).suffix() != "json")
                  {
                    logger.warn(msg("Only JSON files are supported!"));
                  }

                logger.info(msg("Starting pre-processing..."));
                Controller controller(file, parameters_);

                logger.info(msg("Running algorithm..."));
                controller.start();

                logger.info(msg("Done."));
              }
          }
      }

    // processed all files
    emit quit();
  }
private:
  Parameters parameters_;
};

int main(int argc, char *argv[])
{
  Application application(argc, argv);

  QTimer::singleShot(0, &application, SLOT(processArguments()));

  return application.exec();
}

#include "main.moc"
