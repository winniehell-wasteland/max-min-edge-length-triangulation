#include <QFileInfo>

#include "json_parser.h"

#include "controller.h"

Controller::Controller(QFile &input_file, const Parameters &parameters) :
  points_(JSONParser::parse(input_file)),
  intersection_graph_(points_),
  parameters_(parameters)
{
  qxtLog->info(msg("Read %1 points.").arg(points_.size()));
  qxtLog->info(msg("Constructed intersection graph with %1 segments.")
               .arg(std::distance(intersection_graph_.segments_begin(),
                                  intersection_graph_.segments_end())));

  qxtLog->info(msg("Drawing intersection graph..."));
  intersection_graph_.draw(QFileInfo(input_file).completeBaseName(), parameters_);
}

void Controller::start()
{

}
