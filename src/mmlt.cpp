#include "config.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <QElapsedTimer>

#include "utils/controller.h"
#include "utils/logger.h"
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

    po::options_description pos_options("Positional Arguments");
    pos_options.add_options()
            ("input-file,i",
             po::value<std::string>(&settings.input_file),
             "JSON input file")
    ;
    cmd_options.add(pos_options);

    po::positional_options_description pos_options_desc;
    pos_options_desc.add("input-file", 1);

    po::options_description config_options("Config Options");
    init_controller_options(settings, config_options);
    init_drawing_options(settings, config_options);

    cmd_options.add(config_options);

    // read arguments from command line
    po::variables_map arguments;
    po::store(po::command_line_parser(argc, argv)
              .options(cmd_options)
              .positional(pos_options_desc)
              .run(),
              arguments);
    po::notify(arguments);

    if(arguments.count("help")) {
        std::cout << "Usage: mmlt [options] "
                  << pos_options_desc.name_for_position(0)
                  << std::endl
                  << cmd_options
                  << std::endl;

        return EXIT_SUCCESS;
    }

    logger.info( mmlt_msg("Running in directory %1")
                 .arg( QString::fromStdString( fs::current_path().generic_string() ) ) );

    try {
        // invisible options
        init_generator_options(settings, config_options);

        read_config_file(settings, config_options, arguments);
    }
    catch(std::exception& e) {
        logger.error( mmlt_msg("Could not read config file: %1!")
                      .arg( QString::fromUtf8( e.what() ) ) );
        return EXIT_FAILURE;
    }

    if(!arguments.count("input-file"))
    {
        logger.error(mmlt_msg("Invalid number of arguments! Expected input file name."));
        return EXIT_FAILURE;
    }

    // implications
    settings.complete_igraph = settings.complete_igraph || settings.complete_sat;

    QElapsedTimer total_time;
    total_time.start();

    QFile input_file( QString::fromStdString(arguments["input-file"].as<std::string>()) );
    QString file_prefix(
                input_file
                .fileName()
                .left(
                    input_file
                    .fileName()
                    .lastIndexOf('.')
                    )
                );

    Controller controller(file_prefix, input_file, settings);

    if( controller.start() )
    {
        logger.time( mmlt_msg( "controller initialization" ), total_time.elapsed());

        while( controller.iteration() )
        {

        }
    }
    else
    {
        logger.time( mmlt_msg( "controller initialization" ), total_time.elapsed());
    }

    controller.done();
    logger.time( mmlt_msg( "total" ), total_time.elapsed());

    return EXIT_SUCCESS;
}
