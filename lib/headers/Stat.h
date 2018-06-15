//
// Created by andrey on 12.06.18.
//

#ifndef HUFFMAN_COMPRESSION_STATISTIC_H
#define HUFFMAN_COMPRESSION_STATISTIC_H

#include <ostream>
#include <string>
#include <map>

namespace huffman {
    const std::string HEADER_BYTES = "Header bytes";
    const std::string ORIGINAL_BYTES = "Total bytes in original file";
    const std::string COMPRESSED_BYTES = "Total bytes in compressed file";
    const std::string COMPRESSION_RATE = "Compression rate";
    const std::string SPEED = "Speed";
    const std::string TIME = "Time";


    class Stat {
    private:
        Stat() {};

    public:
        size_t header_bytes = 0;
        size_t original_bytes = 0;
        size_t compressed_bytes = 0;
        double compression_rate = 0;
        double speed = 0;
        double time = 0;

        static Stat &instance() {
            static Stat statistic;
            return statistic;
        }

        Stat(Stat const &) = delete;

        Stat &operator=(Stat const &) = delete;

        friend std::ostream &operator<<(std::ostream &s, Stat const &stat) {
            s << ORIGINAL_BYTES << ": " << stat.original_bytes << std::endl;
            s << COMPRESSED_BYTES << ": " << stat.compressed_bytes << std::endl;
            s << HEADER_BYTES << ": " << stat.header_bytes << std::endl;
            s << COMPRESSION_RATE << ": " << stat.compression_rate << std::endl;
            s << TIME << ": " << stat.time * 1000 << " ms" << std::endl;
            s << SPEED << ": " << stat.speed / 1000 / 1000 << " MB/s" << std::endl;
            return s;
        }
    };
}

#endif //HUFFMAN_COMPRESSION_STATISTIC_H
