#include "config.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "utils/logger.h"
#include "utils/point_generator.h"
#include "utils/settings.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

using namespace MMLT;

int main(int argc, char* argv[])
{
    std::cout << mmlt_msg("Build: %1 by %2 (last commit: %3)")
                 .arg(BUILD_DATE)
                 .arg(BUILD_AUTHOR)
                 .arg(COMMIT).toStdString()
                 << std::endl;

    Settings settings;

    po::options_description cmd_options("Command Line Options");
    init_command_line_options(settings, cmd_options);

    po::options_description config_options("Config Options");
    init_generator_options(settings, config_options);

    cmd_options.add(config_options);

    // read arguments from command line
    po::variables_map arguments;
    po::store(po::command_line_parser(argc, argv)
              .options(cmd_options)
              .run(),
              arguments);
    po::notify(arguments);

    if(arguments.count("help")) {
        std::cout << "Usage: mmlt [options] "
                  << std::endl
                  << cmd_options
                  << std::endl;

        return EXIT_SUCCESS;
    }

    logger.info( mmlt_msg("Running in directory %1")
                 .arg( QString::fromStdString( fs::current_path().generic_string() ) ) );

    try {
        // invisible options
        init_controller_options(settings, config_options);
        init_drawing_options(settings, config_options);

        read_config_file(settings, config_options, arguments);
    }
    catch(std::exception& e) {
        logger.error( mmlt_msg("Could not read config file: %1!")
                      .arg( QString::fromUtf8( e.what() ) ) );
        return EXIT_FAILURE;
    }

    PointGenerator::run(settings);
    return EXIT_SUCCESS;
}
