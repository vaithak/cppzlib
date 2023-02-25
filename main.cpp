#include <fstream>
#include "compressor.hpp"

// example usages:
// $ ./cppzlib -c file.txt -> compress file.txt to file.txt.z
// $ ./cppzlib -d file.txt.z -> decompress file.txt.z to file.txt


int main(int argc, char** argv) {
    // parse command line arguments.
    if ((argc != 3) || (argv[1][0] != '-') || (argv[1][1] != 'c' && argv[1][1] != 'd')) {
        std::cerr << "Usage: " << argv[0] << " -c|-d <file>" << std::endl;
        return 1;
    }
    bool compress = (argv[1][1] == 'c');
    std::string filename = argv[2];

    // open input file as input stream.
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Could not open file " << filename << std::endl;
        return 1;
    }

    // open output file as output stream.
    std::string output_filename = filename;
    if (compress) {
        output_filename += ".z";
    } else {
        if (output_filename.size() < 2 || output_filename.substr(output_filename.size() - 2) != ".z") {
            std::cerr << "File " << filename << " does not have .z extension." << std::endl;
            return 1;
        }
        output_filename = output_filename.substr(0, output_filename.size() - 2);
    }
    std::ofstream output(output_filename);
    if (!output) {
        std::cerr << "Could not open file " << output_filename << std::endl;
        return 1;
    }

    // create compressor object.
    cppzlib::Compressor compressor;

    // process input stream.
    int ret = compress ? compressor.compress(input, output) : compressor.decompress(input, output);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

// fix error handling in compressor.cpp.
// create main function in main.cpp.
// add basic readme.
// add examples to readme.
// print metrics after processing.
// finalize readme.



