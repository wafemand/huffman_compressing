//
// Created by andrey on 07.06.18.
//

#ifndef HUFFMAN_COMPRESSION_HUFFMAN_H
#define HUFFMAN_COMPRESSION_HUFFMAN_H

#include <string>
#include "Stat.h"
#include "exceptions.h"

/*
 *   HEADER{[tree size][used_codes size][tree][used_codes]}|MESSAGE{[message size][message]}
 */

namespace huffman {
    void compress(std::istream &input_stream, std::ostream &output_stream);

    void decompress(std::istream &input_stream, std::ostream &output_stream);
}
#endif //HUFFMAN_COMPRESSION_HUFFMAN_H
