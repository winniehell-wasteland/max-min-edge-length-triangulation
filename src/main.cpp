#include "config.h"

#include <QtCore>
#include <QGuiApplication>

#include "controller.h"
#include "logger.h"
#include "parameters.h"

class Application : public QGuiApplication
{
  Q_OBJECT

public:
  Application(int &argc, char **argv) :
    QGuiApplication(argc, argv),
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
                logger.error(mmt_msg("Unknown argument: %1").arg(argument));
              }
          }
        else
          {
            QFile file(argument);

            if(!file.exists())
              {
                logger.error(mmt_msg("File %1 does not exist!").arg(argument));
              }
            else
              {
                logger.info(mmt_msg("Reading file %1...").arg(file.fileName()));

                if(QFileInfo(file).suffix() != "json")
                  {
                    logger.warn(mmt_msg("Only JSON files are supported!"));
                  }

                logger.info(mmt_msg("Starting pre-processing..."));
                Controller controller(file, parameters_);

                logger.info(mmt_msg("Running algorithm..."));
                controller.start();

                logger.info(mmt_msg("Done."));
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
