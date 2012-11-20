#ifndef MMT_PARAMETERS_H
#define MMT_PARAMETERS_H

struct Parameters
{
  Parameters() :
    draw_igraph(false),
    draw_igroups(false)
  {

  }

  bool draw_igraph;
  bool draw_igroups;
};

#endif // MMT_PARAMETERS_H
