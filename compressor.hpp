#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <iostream>
#include <chrono>

namespace cppzlib {
    class Compressor {
        // chunk size for zlib compression.
        const size_t CHUNK_SIZE = 16384;

        // state for storing metrics.
        size_t input_size;
        size_t output_size;
        std::chrono::duration<double> elapsed_time;

    public:
        Compressor() : input_size(0), output_size(0) {}
        static int maybe_print_error_msg(int error_code);
        int compress(std::istream& input, std::ostream& output);
        int decompress(std::istream& input, std::ostream& output);
        ~Compressor() = default;

    }; // class Compressor

} // namespace cppzlib

#endif
