#include "controller.h"

Controller::Controller(const PointSet& points) :
    intersection_graph_(points),
    points_(points)
{
  intersection_graph_.draw("test.svg");
}

void Controller::start()
{

}
