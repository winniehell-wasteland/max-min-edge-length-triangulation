#include <iostream>
#include <fstream>

#include "utils/logger.h"

#include "settings.h"

namespace po = boost::program_options;

namespace MMLT
{

void init_command_line_options(Settings& settings,
                               po::options_description& options)
{
    options.add_options()
            ("config-file,c",
             po::value<std::string>(&settings.config_file)
             ->default_value("config.ini"),

             "configuration file")
            ("help,h", "print help message")
    ;

}

void init_controller_options(Settings& settings,
                             po::options_description& parent_options)
{
    po::options_description options("Controller Options");
    options.add_options()
            ("dump_sat_problems",
             po::value<bool>(&settings.dump_sat_problems)
             ->implicit_value(true)
             ->default_value(false), "dump SAT problems")
            ("max_iterations",
             po::value<int>(&settings.max_iterations)
             ->default_value(20),
             "maximum number of SAT iterations")
    ;
    parent_options.add(options);
}

void init_drawing_options(Settings& settings,
                          po::options_description& parent_options)
{
    po::options_description options("Drawing Options");
    options.add_options()
            ("draw",
             po::value<bool>(&settings.draw)
             ->implicit_value(true)
             ->default_value(false),
             "draw SVGs")
            ("draw_bounds",
             po::value<bool>(&settings.draw_bounds)
             ->implicit_value(true)
             ->default_value(false), "draw bounds")
            ("draw_intersections",
             po::value<bool>(&settings.draw_intersections)
             ->implicit_value(true)
             ->default_value(false), "draw intersections")
            ("draw_sat_solution",
             po::value<bool>(&settings.draw_sat_solution)
             ->implicit_value(true)
             ->default_value(false), "draw SAT solution")
            ("draw_segments",
             po::value<bool>(&settings.draw_segments)
             ->implicit_value(true)
             ->default_value(false), "draw segments")
            ("draw_separators",
             po::value<bool>(&settings.draw_separators)
             ->implicit_value(true)
             ->default_value(false), "draw separators")
            ("draw_triangulation",
             po::value<bool>(&settings.draw_triangulation)
             ->implicit_value(true)
             ->default_value(false), "draw triangulation")
    ;
    parent_options.add(options);
}

void init_generator_options(Settings& settings,
                            po::options_description& parent_options)
{
    po::options_description options("Generator Options");
    options.add_options()
            ("generator_type",
             po::value<std::string>(&settings.generator_type)
             ->default_value("random_in_square"),
             "point generator type")
            ("generator_iterations",
             po::value<int>(&settings.generator_iterations)
             ->default_value(0),
             "number of generated point files")
    ;
    parent_options.add(options);
}

#include <cstdio>

void read_config_file(Settings& settings,
                      boost::program_options::options_description& config_options,
                      boost::program_options::variables_map& arguments)
{
    logger.info( mmlt_msg( "Reading config file %1" )
                 .arg( QString::fromStdString( settings.config_file ) ) );

    std::ifstream config_file;
    config_file.open( settings.config_file.c_str() );
    po::store(po::parse_config_file(config_file, config_options), arguments);
    if( errno != 0 ) {
        throw std::ifstream::failure(strerror(errno));
    }
    po::notify(arguments);
}

}
