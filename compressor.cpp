#include "compressor.hpp"
#include <iostream>
#include <zlib.h>

// Function to print error messages according to error codes from zlib.
// Returns 1 if error code from zlib is < 0, 0 otherwise.
// reference: https://zlib.net/manual.html
int cppzlib::Compressor::maybe_print_error_msg(int error_code) {
    switch (error_code) {
        case Z_ERRNO:
            std::cerr << "Error reading or writing file." << std::endl;
            return 1;
        case Z_STREAM_ERROR:
            std::cerr << "Invalid compression level." << std::endl;
            return 1;
        case Z_DATA_ERROR:
            std::cerr << "Input data was corrupted." << std::endl;
            return 1;
        case Z_MEM_ERROR:
            std::cerr << "Out of memory." << std::endl;
            return 1;
        case Z_BUF_ERROR:
            std::cerr << "Output buffer was too small." << std::endl;
            return 1;
        case Z_VERSION_ERROR:
            std::cerr << "Incompatible zlib version." << std::endl;
            return 1;
        default:
            return 0;
    }
}

// Function to compress input stream to output stream.
// Returns 0 if successful, 1 otherwise.
int cppzlib::Compressor::compress(std::istream& input, std::ostream& output) {
    // initialize zlib stream.
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    // initialize zlib.
    int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return maybe_print_error_msg(ret);
    }
    int flush;

    // allocate memory for input and output buffers.
    unsigned char* input_buffer = new unsigned char[CHUNK_SIZE];
    unsigned char* output_buffer = new unsigned char[CHUNK_SIZE];

    // define lambda function for cleaning up.
    auto cleanup = [&]() -> void {
        deflateEnd(&stream);
        delete[] input_buffer;
        delete[] output_buffer;
    };

    // define lambda function for handling error.
    auto handle_error = [&](int ret) -> int {
        int error = maybe_print_error_msg(ret);
        if (error) {
            cleanup();
        }
        return error;
    };

    // start timer.
    auto start_time = std::chrono::high_resolution_clock::now();

    // read data from input stream and compress it.
    do {
        // read data from input stream and handle errors.
        input.read(reinterpret_cast<char*>(input_buffer), CHUNK_SIZE);
        if (input.bad()) {
            return handle_error(Z_ERRNO);
        }
        stream.avail_in = input.gcount();
        input_size += input.gcount();
        stream.next_in = input_buffer;
        flush = input.eof() ? Z_FINISH : Z_NO_FLUSH;

        // compress data till output buffer is full.
        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = output_buffer;
            ret = deflate(&stream, flush);
            if (ret == Z_STREAM_ERROR) {
                return handle_error(ret);
            }
            output.write(reinterpret_cast<char*>(output_buffer), CHUNK_SIZE - stream.avail_out);
            if (output.bad()) {
                return handle_error(Z_ERRNO);
            }
            output_size += CHUNK_SIZE - stream.avail_out;
        } while (stream.avail_out == 0);
    } while (flush != Z_FINISH);

    // clean up.
    cleanup();

    // stop timer.
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed_time = end_time - start_time;

    return 0;
}

// Function to decompress input stream to output stream.
// Returns 0 if successful, 1 otherwise.
int cppzlib::Compressor::decompress(std::istream& input, std::ostream& output) {
    // initialize zlib stream.
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    // initialize zlib.
    int ret = inflateInit(&stream);
    if (ret != Z_OK) {
        return maybe_print_error_msg(ret);
    }

    // allocate memory for input and output buffers.
    unsigned char* input_buffer = new unsigned char[CHUNK_SIZE];
    unsigned char* output_buffer = new unsigned char[CHUNK_SIZE];

    // define lambda function for cleaning up.
    auto cleanup = [&]() -> void {
        inflateEnd(&stream);
        delete[] input_buffer;
        delete[] output_buffer;
    };

    // define lambda function for handling error.
    auto handle_error = [&](int ret) -> int {
        int error = maybe_print_error_msg(ret);
        if (error) {
            cleanup();
        }
        return error;
    };

    // start timer.
    auto start_time = std::chrono::high_resolution_clock::now();

    // read data from input stream and decompress it.
    do {
        // read data from input stream and handle errors.
        input.read(reinterpret_cast<char*>(input_buffer), CHUNK_SIZE);
        if (input.bad()) {
            return handle_error(Z_ERRNO);
        }
        stream.avail_in = input.gcount();
        input_size += input.gcount();
        stream.next_in = input_buffer;

        // decompress data till output buffer is full.
        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = output_buffer;
            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                return handle_error(ret);
            }
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    return handle_error(ret);
            }
            output.write(reinterpret_cast<char*>(output_buffer), CHUNK_SIZE - stream.avail_out);
            if (output.bad()) {
                return handle_error(Z_ERRNO);
            }
            output_size += CHUNK_SIZE - stream.avail_out;
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    // clean up.
    cleanup();

    // stop timer.
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed_time = end_time - start_time;

    return 0;
}
