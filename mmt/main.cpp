#include "config.h"

#include <QtCore>
#include <QGuiApplication>

#include "controller.h"
#include "parameters.h"

#include "utils/logger.h"

class Application : public QGuiApplication
{
  Q_OBJECT

public:
  Application(int &argc, char **argv) :
    QGuiApplication(argc, argv),
    controller_(0),
    parameters_()
  {
  }

public Q_SLOTS:
  void clean_up()
  {
      if(controller_)
      {
          delete controller_;
      }
  }

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
                  emit quit();
              }
              else
              {
                  logger.info(mmt_msg("Reading file %1...").arg(file.fileName()));

                  if(QFileInfo(file).suffix() != "json")
                  {
                      logger.warn(mmt_msg("Only JSON files are supported!"));
                  }

                  logger.info(mmt_msg("Starting pre-processing..."));
                  controller_ = new Controller(file, parameters_);
                  QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(clean_up()));

                  QObject::connect(controller_, SIGNAL(done()), this, SLOT(quit()));
                  QTimer::singleShot(0, controller_, SLOT(init()));

                  // process only first file
                  break;
              }
          }
      }
  }
private:
  Controller* controller_;
  Parameters  parameters_;
};

int main(int argc, char *argv[])
{
  Application application(argc, argv);

  QTimer::singleShot(0, &application, SLOT(processArguments()));

  return application.exec();
}

#include "main.moc"
