//
// Created by andrey on 12.06.18.
//
#include <memory>
#include <iostream>
#include "../headers/huffman.h"
#include "lib/headers/BitStreamReader.h"
#include "lib/headers/BitStreamWriter.h"
#include "../headers/Tree.h"


namespace huffman {
    const size_t WORD_SIZE = 8;
    const size_t ALPHABET_SIZE = 1ull << WORD_SIZE;


    void write_compressed_tree(Tree &tree, BitStreamWriter &writer) {
        Stat &stat = Stat::instance();
        std::vector<bool> path;
        std::vector<Byte> used_codes;
        tree.compress_tree(path, used_codes);
        auto tree_size = static_cast<uint16_t>(path.size());
        auto used_alphabet_size = static_cast<uint16_t>(used_codes.size());
        writer.set_bit_mode(false);
        writer.write_int(tree_size);
        writer.write_int(used_alphabet_size);
        writer.set_bit_mode(true);
        for (bool bit : path) {
            writer.write_bit(bit);
        }
        writer.set_bit_mode(false);
        for (Byte code : used_codes) {
            writer.write_byte(code);
        }
        writer.flush();
        stat.header_bytes = writer.get_total_byte_count();
    }


    void compress(std::istream &input_stream, std::ostream &output_stream) {
        Stat &stat = Stat::instance();
        auto timer = clock();
        auto start = input_stream.tellg();
        BitStreamReader reader(input_stream);
        BitStreamWriter writer(output_stream);
        std::vector<size_t> counts(ALPHABET_SIZE);
        size_t message_size = 0;

        while (!reader.eof()) {
            Byte byte = reader.read_byte();
            counts[byte]++;
            message_size++;
        }

        Tree tree(counts);

        write_compressed_tree(tree, writer);
        writer.write_int(message_size);
        writer.set_bit_mode(true);
        input_stream.clear();
        input_stream.seekg(start);
        BitStreamReader reader2(input_stream);
        while (!reader2.eof()) {
            writer.write(tree.get_bit_code(reader2.read_byte()));
        }
        writer.set_bit_mode(false);
        writer.flush();

        stat.compressed_bytes = writer.get_total_byte_count();
        stat.original_bytes = message_size;
        stat.compression_rate = 1.0 * stat.original_bytes / stat.compressed_bytes;
        stat.time = (clock() - timer) * 1.0 / CLOCKS_PER_SEC;
        stat.speed = stat.original_bytes / stat.time;
    }


    void read_header(BitStreamReader &reader, std::vector<bool> &path, std::vector<Byte> &used_codes) {
        Stat &stat = Stat::instance();
        reader.set_bit_mode(false);
        size_t tree_size = reader.read_int<uint16_t>();
        size_t used_alphabet_size = reader.read_int<uint16_t>();
        reader.set_bit_mode(true);
        for (size_t i = 0; i < tree_size; ++i) {
            path.push_back(reader.read_bit());
        }
        reader.set_bit_mode(false);
        for (size_t i = 0; i < used_alphabet_size; i++) {
            used_codes.push_back(reader.read_byte());
        }
        stat.header_bytes = reader.get_total_byte_count();
    }


    void decompress(std::istream &input_stream, std::ostream &output_stream) {
        Stat &stat = Stat::instance();
        auto timer = clock();
        BitStreamReader reader(input_stream);
        BitStreamWriter writer(output_stream);
        std::vector<bool> path;
        std::vector<Byte> used_codes;

        try {
            read_header(reader, path, used_codes);
        } catch (end_of_file const &e) {
            throw damaged_file();
        }

        Tree tree;
        tree.decompress_tree(path, used_codes);
        reader.set_bit_mode(false);
        auto message_size = reader.read_int<size_t>();
        reader.set_bit_mode(true);
        writer.set_bit_mode(false);
        for (size_t i = 0; i < message_size; ++i) {
            while (!tree.is_list()) {
                bool bit = reader.read_bit();
                tree.apply_bit(bit);
            }
            writer.write_byte(tree.get_word());
        }
        reader.set_bit_mode(false);

        stat.compressed_bytes = reader.get_total_byte_count();
        stat.original_bytes = message_size;
        stat.compression_rate = 1.0 * stat.original_bytes / stat.compressed_bytes;
        stat.time = (clock() - timer) * 1.0 / CLOCKS_PER_SEC;
        stat.speed = stat.original_bytes / stat.time;
    }
}
