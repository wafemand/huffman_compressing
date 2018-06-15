#include <iostream>
#include <fstream>
#include <iterator>
#include <cstring>
#include "huffman.h"

using namespace std;
using namespace huffman;


struct invalid_argument_exception : exception {
    const char *what() const noexcept override {
        return "Invalid arguments";
    }
};


void print_help() {
    cout << "Usage:\n"
            "    huffman [compress|decompress] <input file> <output file>" << endl;
}


void do_compress(int argc, const char **argv) {
    if (argc < 3) {
        throw invalid_argument_exception();
    }
    ifstream input(argv[2], std::ifstream::binary);
    ofstream output(argv[3], std::ofstream::binary);
    compress(input, output);
}


void do_decompress(int argc, const char **argv) {
    if (argc < 3) {
        throw invalid_argument_exception();
    }
    ifstream input(argv[2], std::ifstream::binary);
    ofstream output(argv[3], std::ofstream::binary);
    decompress(input, output);
}


int main(int argc, const char **argv) {
    if (argc == 0) {
        print_help();
        return 0;
    }

    try {
        if (strcmp(argv[1], "compress") == 0) {
            do_compress(argc, argv);
        } else if (strcmp(argv[1], "decompress") == 0) {
            do_decompress(argc, argv);
        }
    } catch (invalid_argument_exception const &e) {
        cout << e.what() << endl;
        print_help();
    } catch (damaged_file const &e) {
        cout << e.what() << endl;
    } catch (ios_base::failure const &e) {
        cout << "Couldn't open file" << endl;
        cout << e.what() << endl;
        cout << e.code() << endl;
    }
    cout << endl << Stat::instance();
}