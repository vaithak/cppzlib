#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <iostream>
#include <chrono>

namespace cppzlib {
    // struct to store metrics.
    struct Metrics {
        size_t input_size;
        size_t output_size;
        std::chrono::duration<double> elapsed_time;
    };
    
    class Compressor {
        // chunk size for zlib compression.
        const size_t CHUNK_SIZE = 16384;

        // state for storing metrics.
        Metrics metrics;

    public:
        Compressor();
        static int maybe_print_error_msg(int error_code);
        int compress(std::istream& input, std::ostream& output);
        int decompress(std::istream& input, std::ostream& output);
        const Metrics& get_metrics() { return metrics;}
        ~Compressor() = default;

    }; // class Compressor

} // namespace cppzlib

#endif
