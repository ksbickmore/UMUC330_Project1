/**
 * @file main.cpp
 * @brief The driver for the parser application
 * @author Kristopher Bickmore
 * @date November 20, 2016
 *
 * \mainpage Project 1 File Parsing
 * \author Kristopher Bickmore
 * \date November 20, 2016
 * \section Build
 * \subsection Windows
 * Tested on:\n
 * Windows 10 Pro 64-bit\n
 * Version 1607\n
 * OS Build 14393.447\n
 *
 * Microsoft Visual Studio Community 2015\n
 * Version 14.0.25431.01 Update 3
 *
 * \subsection Linux
 * Tested on:\n
 * Ubuntu 64-bit\n
 * Version 16.04.1 LTS xenial
 *
 * GNU g++\n
 * Version 5.4.0 20160609
 *
 * Eclipse\n
 * Version Neon.1a\n
 * Release 4.6.1\n
 * Build id: 20161007-1200
 *
 * go to Project Properties -> c/c++ Build -> settings -> Tool Settings -> GCC C++ Compiler\n
 * Under Dialect select ISO c++1y(-std=c++1y)
 *
 * go to Project Properties -> c/c++ Build -> settings -> Tool Settings -> GCC C++ Linker\n
 * Under Libraries add stdc++fs
 *
 * \subsection MINGW32
 * There are bugs in the MINGW32 compiler that mishandle c++11 functionality such as std::stoi or std::to_string.
 * \subsection MINGW64
 * There are bugs in the MINGW64 installer that did not allow me to ever properly install.  Source code installation\n
 * failed do to missing libraries.\n
 * \subsection Cygwin
 * The Cygwin g++ compiler fails to properly resolve templates.\n
 * for instance passing a string into the path constructor does not resolve properly.\n
 * template< class Source >\n
 * path( const Source& source );\n
 * \section Usage
 * The program can be run with no options or a combination of the below.\n\n

 *  Usage: parser(.exe) <option(s)>\n
    Lexer/Parser for files using the grammar specified for CMSC 330 Project 1.\n\n

    Optional Options:\n
        -h,--help                       Show this help message.\n
        -d,--directory DIRECTORY        Specify Path holding files to test. Cannot use with --file.\n
                                        (Defaults to ..\\test_input_files\\ / ../test_input_files/)\n
        -o,--output DIRECTORY           Specify Directory for output files.\n
                                        (Defaults to ..\\test_input_files\\ / ../test_input_files/)\n
        -f,--file FILE                  Use to Parse only a single file. Cannot use with --directory\n
        -p,--print                      Print output to screen.\n
 *
 */
#include <algorithm>

#ifdef _WIN32
#include <experimental\filesystem>
#elif __linux__
#include <experimental/filesystem>
#endif
#include <iostream>
#include <vector>

#include "Parser.h"
#include "stringhelper.h"

using namespace std;

/**
 * Prints a usage string to let the user know the available options.
 * @param name the name of the program.
 */
static void show_usage(string name) {
    string description("Lexer/Parser for files using the grammar specified for CMSC 330 Project 1.\n\n");
    cerr << "Usage: " << name << " <option(s)>\n" << description
        << "Optional Options:\n"
        << "\t-h,--help\t\t\tShow this help message.\n"
        << "\t-d,--directory DIRECTORY\tSpecify Path holding files to test. Cannot use with --file.\n\t\t\t\t\t(Defaults to ..\\test_input_files\\ / ../test_input_files/)\n"
        << "\t-o,--output DIRECTORY\t\tSpecify Directory for output files.\n\t\t\t\t\t(Defaults to ..\\test_input_files\\ / ../test_input_files/)\n"
        << "\t-f,--file FILE\t\t\tUse to Parse only a single file. Cannot use with --directory\n"
        << "\t-p,--print\t\t\tPrint output to screen.\n" << endl;
}

/**
 * The main driver for the application
 * @param argc number of arguments
 * @param argv the array of arguments
 * @return an int
 */
int main(int argc, char *argv[]) {
    if (argc > 7) {
        show_usage(argv[0]);
        exit(1);
    }
    if (argc == 1){
        cout << "Use the -h option for more details" << endl;
    }
#ifdef _WIN32
    std::string testDirectory("..\\test_input_files");
    set<char> delimiters{ '\\' };
#elif __linux__
    std::string testDirectory("../test_input_files");
    set<char> delimiters{ '/' };
#endif

    // default output location
    string outputDirectory(testDirectory);
    string singleFileName("");
    
    bool directoryCheck = false;
    bool fileCheck = false;
    bool printCheck = false;

    // Handle Options
    for (int i = 0; i < argc; ++i) {
        string arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            show_usage(argv[0]);
            exit(1);
        }
        else if (arg == "-d" || arg == "--directory") {
            if (fileCheck == true) {
                cout << "--directory and --file are mutually exclusive." << endl;
                exit(1);
            }
            directoryCheck = true;
            if (i + 1 < argc) {
                testDirectory = argv[++i];
            }
            else {
                cout << "--directory requires one argument" << endl;
                exit(1);
            }
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputDirectory = argv[++i];
            }
            else {
                cout << "--output requires one argument" << endl;
                exit(1);
            }
        }
        else if (arg == "-f" || arg == "--file") {
            if (directoryCheck == true) {
                cout << "--directory and --file are mutually exclusive." << endl;
                exit(1);
            }
            fileCheck = true;
            if (i + 1 < argc) {
                singleFileName = argv[++i];
            }
            else {
                cout << "--file requires one argument" << endl;
                exit(1);
            }
        }
        else if (arg == "-p" || arg == "--print") {
            printCheck = true;
        }
    }

    // if we only want one file
    if (fileCheck) {
        vector <string> pathSplit = StringHelper::splitpath(singleFileName, delimiters);
#ifdef _WIN32
        string outfile(outputDirectory + "\\OUTPUT_" + pathSplit.back());
#elif __linux__
        string outfile(outputDirectory + "/OUTPUT_" + pathSplit.back());
#endif
        if (printCheck) {
            cout << "\n\n*******************************************************\nPARSING: "
                << singleFileName
                << "\n*******************************************************\n\n\n" << endl;;
        }
        try {

            Parser parser(std::experimental::filesystem::path(singleFileName), outfile, printCheck);
            parser.file();
        }
        catch (runtime_error& e) {
            cout << "Caught Exception: " << e.what() << endl;
            exit(1);
        }
    }
    else {
        // grab all files in the input directory
        for (auto& dirEntry : experimental::filesystem::directory_iterator(testDirectory)) {
            vector <string> pathSplit = StringHelper::splitpath(dirEntry.path().string(), delimiters);
            if (!pathSplit.back().find("OUTPUT_"))
                continue;
#ifdef _WIN32
            string outfile(outputDirectory + "\\OUTPUT_" + pathSplit.back());
#elif __linux__
            string outfile(outputDirectory + "/OUTPUT_" + pathSplit.back());
#endif
            if (printCheck) {
                cout << "\n\n*******************************************************\nPARSING: "
                    << dirEntry
                    << "\n*******************************************************\n\n\n" << endl;;
            }
            try {
                Parser parser(dirEntry.path(), outfile, printCheck);
                parser.file();
            }
            catch (runtime_error &e) {
                cout << "Caught Exception: " << e.what() << endl;
                exit(1);
            }
            
        }
    }
    cout << "... Finished\nCheck " << outputDirectory << " for all output files."<< endl;
    return 0;
}
