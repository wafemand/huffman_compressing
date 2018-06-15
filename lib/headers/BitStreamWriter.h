//
// Created by andrey on 09.06.18.
//

#ifndef HUFFMAN_COMPRESSION_BIT_WRITER_H
#define HUFFMAN_COMPRESSION_BIT_WRITER_H

#include <cstddef>
#include <ostream>
#include "BitCode.h"
#include "bitset"


namespace huffman {


    inline std::string to2(Byte a){
        std::string res ;
        while (a > 0){
            res.push_back('0' + (a & 1));
            a /= 2;
        }
        while (res.size() < 8){
            res.push_back('0');
        }
        return res;
    }


    class BitStreamWriter {
        static const size_t BYTE = 8;
        static const size_t BUF_CAP = 1024 * 1;

        size_t total_byte_count = 0;
        std::ostream &output_stream;
        bool bit_mode = false;
        size_t bit_shift = 0;
        size_t buf_pos = 0;      // <index of last byte> + 1;
        Byte *buf = nullptr;

        void fix_buf() {
            if (buf_pos >= BUF_CAP) {
                flush();
            }
        }

        void inc_buf_pos() {
            total_byte_count++;
            buf_pos++;
            fix_buf();
        }

    public:
        explicit BitStreamWriter(std::ostream &stream)
                : output_stream(stream), buf(new Byte[BUF_CAP]) {
            output_stream.exceptions(std::ostream::badbit);
            std::fill(buf, buf + BUF_CAP, 0);
        }

        void write_bit(bool bit) { // use only in bit mode, else UB
            buf[buf_pos] |= bit << bit_shift;
            bit_shift++;
            if (bit_shift == BYTE) {
                bit_shift = 0;
                inc_buf_pos();
            }
        }

        void write_byte(Byte byte) { // use only in normal mode, else UB
            buf[buf_pos] = byte;
            inc_buf_pos();
        }

        template<typename T>
        void writeInt(T val) { // use only in normal mode, else UB
            for (int i = 0; i < sizeof(T); ++i) {
                write_byte(val >> (i * BYTE));
            }
        }

        void write(BitCode code) { // use only in bit mode, else UB
            size_t cur_rest = (BYTE - bit_shift);

            while (code.length >= cur_rest) {
                buf[buf_pos] |= code.bits << bit_shift;
                inc_buf_pos();
                code.bits >>= cur_rest;
                code.length -= cur_rest;
                cur_rest = BYTE;
                bit_shift = 0;
            }

            buf[buf_pos] |= code.bits << bit_shift;
            bit_shift += code.length;
        }

        void flush() { // use only in normal mode, else UB
            auto *char_buf = reinterpret_cast<char *>(buf);   // because i want unsigned bytes
            output_stream.write(char_buf, buf_pos);           // but .read need char*
            std::fill(buf, buf + BUF_CAP, 0);
            buf_pos = 0;
        }

        ~BitStreamWriter() {
            flush();
            output_stream.flush();
            delete[] buf;
        }

        void set_bit_mode(bool mode) {
            if (!mode) {
                if (bit_shift > 0) {
                    inc_buf_pos();
                    bit_shift = 0;
                }
            }
            bit_mode = mode;
        }

        size_t get_total_byte_count() const {
            return total_byte_count;
        }
    };
}


#endif //HUFFMAN_COMPRESSION_BIT_WRITER_H
