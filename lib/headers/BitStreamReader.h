//
// Created by andrey on 09.06.18.
//

#ifndef HUFFMAN_COMPRESSION_BITFILEREADER_H
#define HUFFMAN_COMPRESSION_BITFILEREADER_H

#include <cstddef>
#include <istream>
#include "BitCode.h"


namespace huffman {


    class BitStreamReader {
        static const size_t BYTE = 8;
        static const size_t BUF_CAP = 1024 * 128;

        size_t total_byte_count = 0;
        std::istream &input_stream;
        bool bit_mode = false;
        size_t bit_shift = 0;
        size_t buf_pos = 0;
        size_t buf_end = 0;
        Byte *buf = nullptr;

        void update_buf(){
            auto *char_buf = reinterpret_cast<char *>(buf); // because i want unsigned bytes
            input_stream.read(char_buf, BUF_CAP);           // but .read need char*
            buf_pos = 0;
            buf_end = static_cast<size_t>(input_stream.gcount());
        }

        void fix_buf() {
            if (buf_pos >= BUF_CAP) {
                update_buf();
            }
        }

        void inc_buf_pos(){
            total_byte_count++;
            buf_pos++;
            fix_buf();
        }

    public:
        explicit BitStreamReader(std::istream &stream)
        // не выделяй руками память, используй unique_ptr, shared_ptr, std::array, std::vector
                : input_stream(stream), buf(new Byte[BUF_CAP]) {
            input_stream.exceptions(std::istream::badbit);
            update_buf();
        }

        bool read_bit() {
            auto res = (buf[buf_pos] & (1ull << bit_shift)) > 0;
            bit_shift++;
            if (bit_shift == BYTE){
                inc_buf_pos();
                bit_shift = 0;
            }
            return res;
        }

        Byte read_byte() {
            Byte res = buf[buf_pos];
            inc_buf_pos();
            return res;
        }

        template<typename T>
        T read_int() {
            T res = 0;
            for (int i = 0; i < sizeof(T); ++i) {
                T cur = read_byte();
                res += cur << (i * BYTE);
            }
            return res;
        }

        size_t get_total_byte_count() const {
            return total_byte_count;
        }

        void set_bit_mode(bool mode){
            if (!mode){
                if (bit_shift > 0){
                    inc_buf_pos();
                    bit_shift = 0;
                }
            }
            bit_mode = mode;
        }

        bool eof() const {
            return buf_pos == buf_end;
        }

        ~BitStreamReader() {
            delete[] buf;
        }
    };

}
#endif //HUFFMAN_COMPRESSION_BITFILEREADER_H
