//
// Created by andrey on 11.06.18.
//

#ifndef HUFFMAN_COMPRESSION_EXCEPTIONS_H
#define HUFFMAN_COMPRESSION_EXCEPTIONS_H

#include <string>
#include <exception>

namespace huffman {
    struct end_of_file : std::exception {
        const char *what() const noexcept override {
            return "File damaged: Unexpected end of file.";
        }
    };


    struct damaged_file : std::exception {
        const char *what() const noexcept override {
            return "File damaged";
        }
    };
}

#endif //HUFFMAN_COMPRESSION_EXCEPTIONS_H
