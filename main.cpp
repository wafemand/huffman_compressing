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
    cerr << "Usage:\n"
            "  with file names:"
            "    huffman_utility [compress|decompress] <input file> <output file>\n"
            "  or with stdin/stdout"
            "    huffman_utility [compress|decompress]"
            "Examples:"
            "  huffman_utility compress file.txt file.compressed"
            "  huffman_utility decompress <file.compressed >/dev/null" << endl;
}


template<class Worker>
void do_(Worker worker, int argc, const char **argv) {
    if (argc == 2) {
        worker(cin, cout);
    } else if (argc == 4) {
        ifstream input(argv[2], std::ifstream::binary);
        input.exceptions(ifstream::failbit);

        ofstream output(argv[3], std::ofstream::binary);
        output.exceptions(ofstream::failbit);

        worker(input, output);
    } else {
        throw invalid_argument_exception();
    }
}


int main(int argc, const char **argv) {
    if (argc == 1) {
        print_help();
        return 0;
    }
    try {
        if (strcmp(argv[1], "compress") == 0) {
            do_(compress, argc, argv);
        } else if (strcmp(argv[1], "decompress") == 0) {
            do_(decompress, argc, argv);
        }
        cerr << endl << Stat::instance();
    } catch (invalid_argument_exception const &e) {
        cerr << e.what() << endl;
        print_help();
    } catch (damaged_file const &e) {
        cerr << e.what() << endl;
    } catch (ios_base::failure const &e) {
        cerr << "Couldn't open file" << endl;
        cerr << e.what() << endl;
        cerr << e.code() << endl;
    }
}