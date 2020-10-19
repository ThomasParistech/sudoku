/*********************************************************************************************************************
 * File : main.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>

#include <sudoku_solver.h>

namespace boost_po = boost::program_options;
namespace bfs = boost::filesystem;

struct Config
{
    std::string input_filename_csv;
    std::string output_dir_path;
};

/// @brief Utility function to parse command line attributes
bool parse_command_line(int argc, char *argv[], Config &config)
{
    const std::string short_program_desc(
        "");

    const std::string long_program_desc(
        short_program_desc +
        "");

    boost_po::options_description options_desc;
    boost_po::options_description base_options("Base options");
    // clang-format off
    base_options.add_options()
        ("help,h", "Produce help message.")
        ("input-csv,i", boost_po::value<std::string>(&config.input_filename_csv), "Input csv file containing the sudoku grid to solve.")
        ("output-directory,o", boost_po::value<std::string>(&config.output_dir_path)->default_value("/tmp/sudoku"), "Output directory to export the sudoku solution.")
        ;
    // clang-format on

    options_desc.add(base_options);

    boost_po::variables_map vm;
    try
    {
        boost_po::store(boost_po::command_line_parser(argc, argv).options(options_desc).run(), vm);
        boost_po::notify(vm);
    }
    catch (boost_po::error &e)
    {
        std::cerr << short_program_desc << std::endl;
        std::cerr << options_desc << std::endl;
        std::cerr << "Parsing error:" << e.what() << std::endl;
        return false;
    }

    if (vm.count("help"))
    {
        std::cout << short_program_desc << std::endl;
        std::cout << options_desc << std::endl;
        return false;
    }

    if (!bfs::exists(config.output_dir_path))
    {
        try
        {
            bfs::create_directories(config.output_dir_path);
        }
        catch (bfs::filesystem_error &e)
        {
            std::cerr << "Unable to create folder" << config.output_dir_path << std::endl;
            return false;
        }
    }
    if (!bfs::exists(config.input_filename_csv))
    {
        std::cerr << "The input csv doesn't exist: " << config.input_filename_csv << std::endl;
        return false;
    }

    std::cout << long_program_desc << std::endl;
    return true;
}

int main(int argc, char **argv)
{
    Config config;
    if (!parse_command_line(argc, argv, config))
        return 1;

    SudokuSolver solver(config.output_dir_path);
    if (!solver.solve_grid(config.input_filename_csv))
        return 2;

    return 0;
}
