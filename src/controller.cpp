#include <QFileInfo>

#include "json_parser.h"

#include "controller.h"

Controller::Controller(QFile &input_file, const Parameters &parameters) :
  points_(JSONParser::parse(input_file)),
  intersection_graph_(points_),
  parameters_(parameters)
{
  qxtLog->info(msg("Constructed intersection graph with %1 segments.")
               .arg(intersection_graph_.segments().size()));

  intersection_graph_.draw(QFileInfo(input_file).completeBaseName(), parameters_);
}

void Controller::start()
{

}
