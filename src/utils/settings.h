#pragma once
#ifndef MMLT_UTILS_SETTINGS_H
#define MMLT_UTILS_SETTINGS_H

#include <boost/program_options.hpp>

namespace MMLT
{
    struct Settings
    {
        std::string  config_file;
        std::string  input_file;

        bool         draw;
        bool         draw_bounds;
        bool         draw_intersections;
        bool         draw_sat_solution;
        bool         draw_segments;
        bool         draw_separators;
        bool         draw_triangulation;

        bool         dump_sat_problems;

        int          max_iterations;

        std::string  generator_type;
        int          generator_iterations;
    };

    void init_command_line_options(Settings& settings,
                                   boost::program_options::options_description& options);

    void init_controller_options(Settings& settings,
                                 boost::program_options::options_description& options);

    void init_drawing_options(Settings& settings,
                              boost::program_options::options_description& options);

    void init_generator_options(Settings& settings,
                                boost::program_options::options_description& options);

    void read_config_file(Settings& settings,
                          boost::program_options::options_description& config_options,
                          boost::program_options::variables_map& arguments);
}

#endif // MMLT_UTILS_SETTINGS_H
