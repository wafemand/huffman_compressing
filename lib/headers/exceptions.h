//
// Created by andrey on 11.06.18.
//
#pragma once

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
