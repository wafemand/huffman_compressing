//
// Created by andrey on 09.06.18.
//

#ifndef HUFFMAN_COMPRESSION_BITCODE_H
#define HUFFMAN_COMPRESSION_BITCODE_H


#include <stdint-gcc.h>
#include <glob.h>
#include <iosfwd>
#include <ostream>


namespace huffman {
    typedef uint8_t Byte;
    
    
    struct BitCode {
        uint64_t bits = 0;
        size_t length = 0;
        static const size_t MAX_LENGTH = sizeof(bits) * 8;

        void push_back(bool bit) {
            bits |= (uint64_t(bit) << length);
            length++;
        }

        void pop_back() {
            length--;
            bits &= (1ull << length) - 1;
        }

        bool get_bit(size_t index) const {
            return static_cast<bool>((bits >> index) & 1);
        }

        friend std::ostream &operator<<(std::ostream &stream, BitCode const &bit_code) {
            for (size_t i = 0; i < bit_code.length; i++) {
                stream << bit_code.get_bit(i);
            }
            return stream;
        }
    };
}

#endif //HUFFMAN_COMPRESSION_BITCODE_H
