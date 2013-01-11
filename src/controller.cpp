#include <QDebug>
#include <QFileInfo>

#include "json_parser.h"
#include "mmt_utils.h"

#include "controller.h"

Controller::Controller(QFile &input_file, const Parameters &parameters) :
  points_(JSONParser::parse(input_file)),
  segments_(points_),
  intersection_graph_(points_, segments_),
  parameters_(parameters)
{
    logger.info(msg("Read %1 points.").arg(points_.size()));
    logger.info(msg("Constructed %1 segments.").arg(segments_.size()));

    if(parameters.draw_igraph)
    {
        logger.info(msg("Drawing intersection graph..."));
        intersection_graph_.draw_igraph(QFileInfo(input_file).completeBaseName());
    }

    if(parameters.draw_igroups)
    {
        logger.info(msg("Drawing intersection groups..."));
        intersection_graph_.draw_igroups(QFileInfo(input_file).completeBaseName());
    }
}

void Controller::start()
{

}
