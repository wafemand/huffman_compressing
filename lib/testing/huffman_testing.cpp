//
// Created by andrey on 11.06.18.
//

#include "gtest/gtest.h"
#include "huffman.h"
#include <string>
#include <fstream>


using namespace std;
using namespace huffman;


#define TEMP_DIR "tests/"


void compress_decompress(const string &filename){
    string input_file = TEMP_DIR + filename;
    string compressed_file = TEMP_DIR + filename + ".huffman";
    string decompressed_file = TEMP_DIR"decompressed." + filename;
    string diff_command = "diff " + input_file + " " + decompressed_file;
    string rm_compressed = "rm " + compressed_file;
    string rm_decompressed = "rm " + decompressed_file;

    ifstream in(input_file, std::ifstream::binary);
    ofstream out(compressed_file, std::ofstream::binary);
    compress(in, out);
    in.close();
    out.close();

    ifstream din(compressed_file, std::ifstream::binary);
    ofstream dout(decompressed_file, std::ofstream::binary);
    decompress(din, dout);

    int diff_ret = system(diff_command.c_str());
    ASSERT_EQ(diff_ret, 0);
    system(rm_compressed.c_str());
    system(rm_decompressed.c_str());
}


TEST(correctness, png_test) {
    compress_decompress("pic.png");
}


TEST(correctness, random_file_test) {
    system("dd if=/dev/urandom of="TEMP_DIR"random bs=1M count=80");
    compress_decompress("random");
    system("rm "TEMP_DIR"random");
}


TEST(correctness, null_file){
    system("dd if=/dev/zero of="TEMP_DIR"null bs=1M count=80");
    compress_decompress("null");
    system("rm "TEMP_DIR"null");
}


TEST(correctness, empty_file){
    system("dd if=/dev/zero of="TEMP_DIR"empty bs=1M count=0");
    compress_decompress("empty");
    system("rm "TEMP_DIR"empty");
}


TEST(correctness, decompress_incorrect){
    stringstream in("something wrong");
    stringstream out;

    EXPECT_ANY_THROW(decompress(in, out));
}


TEST(correctness, simple_test){
    stringstream in("simple test");
    stringstream compr;
    stringstream decompr;

    compress(in, compr);

    string st = compr.str();
    compr = stringstream(compr.str());

    decompress(compr, decompr);

    EXPECT_EQ(in.str(), decompr.str());
}
