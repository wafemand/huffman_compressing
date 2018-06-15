//
// Created by andrey on 08.06.18.
//

#ifndef HUFFMAN_COMPRESSION_TREE_H
#define HUFFMAN_COMPRESSION_TREE_H


#include <glob.h>
#include <cstdint>
#include <vector>
#include <cassert>
#include "BitCode.h"


namespace huffman {

    class Tree {
    public:
        static const size_t WORD_SIZE = 8;
        static const size_t WORD_NUMBER = 1u << WORD_SIZE;

        void swap(Tree &other);

    private:
        struct Node {
            Byte word = 0;

            Node *sons[2] = {nullptr, nullptr};

            /* Node *left = nullptr;
             Node *right = nullptr;*/

            Node(Byte wrd) : word(wrd) {}

            Node(Node *left, Node *right) {
                sons[0] = left;
                sons[1] = right;
                assert(left != nullptr && right != nullptr);
            }

            ~Node() {
                delete sons[0];
                delete sons[1];
            }
        };

        struct CountedNode {
            Tree::Node *node = nullptr;
            size_t count = 0;

            CountedNode(Node *node, size_t count) : node(node), count(count) {};
        };

        struct CountedNodeComparator {
            bool operator()(CountedNode a, CountedNode b) {
                return a.count > b.count;
            }
        };

        Node *root = nullptr;
        Node *current_node = nullptr;
        std::vector<BitCode> codes;

        void dfs(Node *cur, BitCode code);

        void dfs_compress(Node *cur, std::vector<bool> &path, std::vector<Byte> &used_codes);

        void dfs_decompress(Node *cur,
                            size_t &pos_in_path,
                            size_t &pos_in_used,
                            std::vector<bool> const &path,
                            std::vector<Byte> const &used_codes);

        void build_tree(std::vector<size_t> const &counts);

    public:
        Tree() = default;

        explicit Tree(std::vector<size_t> const &counts);

        Tree(Tree const &other) = delete;

        Tree(Tree &&other) noexcept;

        ~Tree() {
            delete root;
        }


        bool is_list() {
            return current_node->sons[0] == nullptr;
        }

        Byte get_word() {
            assert(is_list());
            Byte res = current_node->word;
            current_node = root;
            return res;
        }

        void apply_bit(bool bit) {
            current_node = current_node->sons[bit];
        }

        BitCode get_bit_code(int word) {
            return codes[word];
        }

        void compress_tree(std::vector<bool> &path, std::vector<Byte> &used_codes);

        void decompress_tree(const std::vector<bool> &path, const std::vector<Byte> &used_codes);
    };
}

#endif //HUFFMAN_COMPRESSION_TREE_H
