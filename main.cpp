#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "compressor.hpp"

// example usages:
// $ ./cppzlib -c file.txt -> compress file.txt to file.txt.z
// $ ./cppzlib -d file.txt.z -> decompress file.txt.z to file.txt

// function to pretty print bytes - returns a pair of the number of units and the unit.
std::pair<double, std::string> pretty_bytes(size_t bytes)
{
    std::vector<std::string> suffixes{"B", "KB", "MB", "GB", "TB"};
    size_t suffix_index = 0;
    double remaining_bytes = bytes;
    while (remaining_bytes > 1024 && suffix_index < suffixes.size() - 1) {
        remaining_bytes /= 1024;
        suffix_index++;
    }
    return {remaining_bytes, suffixes[suffix_index]};
}

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

    // fetch metrics.
    const cppzlib::Metrics& metrics = compressor.get_metrics();
    std::pair<double, std::string> input_bytes = pretty_bytes(metrics.input_size);
    std::pair<double, std::string> output_bytes = pretty_bytes(metrics.output_size);

    // print metrics.
    std::cout.setf(std::ios::fixed | std::ios::showpoint);
    std::cout.precision(2);
    std::cout << std::left << std::setw(25)
        << "Input file size: " << input_bytes.first << " " << input_bytes.second << std::endl;
    if (compress) {
        std::cout << std::left << std::setw(25) 
            << "Compressed file size: " << output_bytes.first << " " << output_bytes.second << std::endl;
        std::cout << std::left << std::setw(25) 
            << "Compression ratio: " << (double)metrics.input_size / metrics.output_size << std::endl;
    } else {
        std::cout << std::left << std::setw(25) 
            << "Decompressed file size: " << output_bytes.first << " " << output_bytes.second << std::endl;
    }
    std::cout << std::left << std::setw(25) 
        << "Elapsed time: " << metrics.elapsed_time.count() * 1000 << " ms" << std::endl;
    std::pair<double, std::string> throughput = pretty_bytes(metrics.input_size / metrics.elapsed_time.count());
    std::cout << std::left << std::setw(25) 
        << "Throughput: " << throughput.first << " " << throughput.second << "/s" << std::endl;
        
    return 0;
}
