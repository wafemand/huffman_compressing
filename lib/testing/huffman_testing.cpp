//
// Created by andrey on 11.06.18.
//

#include "gtest/gtest.h"
#include "huffman.h"
#include <string>
#include <fstream>


using namespace std;
using namespace huffman;


void string_compress_decompress(const string &st){
    stringstream in(st);
    stringstream compressed;
    stringstream decompressed;

    compress(in, compressed);

    compressed = stringstream(compressed.str());

    decompress(compressed, decompressed);

    EXPECT_EQ(in.str(), decompressed.str());
}


TEST(correctness, big_random) {
    string random;
    for (int i = 0; i < 100000000; i++){
        random.push_back(rand());
    }
    string_compress_decompress(random);
}


TEST(correctness, big_null){
    string null;
    for (int i = 0; i < 100000000; i++){
        null.push_back(rand());
    }
    string_compress_decompress(null);
}


TEST(correctness, empty){
    string_compress_decompress("");
}


TEST(correctness, decompress_incorrect){
    stringstream in("this is not compressed string");
    stringstream out;

    EXPECT_ANY_THROW(decompress(in, out));
}


TEST(correctness, simple_test){
    string_compress_decompress("Simple string for simple test");
}
