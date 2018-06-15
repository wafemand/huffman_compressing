//
// Created by andrey on 08.06.18.
//

#include "../headers/Tree.h"
#include "../headers/exceptions.h"
#include <queue>
#include <cassert>


using namespace std;
using namespace huffman;


typedef Tree::Word word;


void throw_if(bool cond) {
    if (cond) {
        throw damaged_file();
    }
}


void Tree::dfs(Node *cur, BitCode code) {
    if (cur->sons[0] != nullptr) {

        assert(cur->sons[1] != nullptr);

        code.push_back(false);
        dfs(cur->sons[0], code);
        code.pop_back();

        code.push_back(true);
        dfs(cur->sons[1], code);
        code.pop_back();
    } else {
        codes[cur->word] = code;
    }
}


void Tree::build_tree(std::vector<size_t> const &counts) {
    priority_queue<
            CountedNode,
            vector<CountedNode>,
            CountedNodeComparator> pq;
    for (size_t cur_word = 0; cur_word < WORD_NUMBER; ++cur_word) {
        if (counts[cur_word] > 0) {
            pq.emplace(new Node(static_cast<Word>(cur_word)), counts[cur_word]);
        }
    }
    while (pq.size() > 1) {
        CountedNode first = pq.top();
        pq.pop();
        CountedNode second = pq.top();
        pq.pop();
        pq.emplace(new Node(first.node, second.node), first.count + second.count);
    }
    if (!pq.empty()) {
        root = pq.top().node;
        codes.resize(WORD_NUMBER);
        dfs(root, BitCode());
    } else {
        root = nullptr;
    }
    current_node = root;
}


void Tree::compress_tree(std::vector<bool> &path, std::vector<uint8_t> &used_codes) {
    if (root != nullptr) {
        dfs_compress(root, path, used_codes);
    }
}


void Tree::dfs_compress(Tree::Node *cur, std::vector<bool> &path, std::vector<uint8_t> &used_codes) {
    if (cur->sons[0] != nullptr) {
        assert(cur->sons[1] != nullptr);
        path.push_back(false);
        dfs_compress(cur->sons[0], path, used_codes);
        path.push_back(false);
        dfs_compress(cur->sons[1], path, used_codes);
    } else {
        used_codes.push_back(cur->word);
    }
    path.push_back(true);
}


void Tree::decompress_tree(std::vector<bool> const &path, std::vector<uint8_t> const &used_codes) {
    if (path.empty()) {
        current_node = root = nullptr;
        return;
    }
    current_node = root = new Node(0);
    size_t pos_in_path = 0;
    size_t pos_in_used = 0;

    dfs_decompress(root, pos_in_path, pos_in_used, path, used_codes);
    codes.resize(WORD_NUMBER);
    dfs(root, BitCode());
}


void Tree::dfs_decompress(Tree::Node *cur,
                          size_t &pos_in_path,
                          size_t &pos_in_used,
                          std::vector<bool> const &path,
                          std::vector<uint8_t> const &used_codes) {
    if (!path[pos_in_path]) {
        for (int i : {0, 1}) {
            cur->sons[i] = new Node(0);
            pos_in_path++;
            throw_if(pos_in_path >= path.size());
            dfs_decompress(cur->sons[i], pos_in_path, pos_in_used, path, used_codes);

            throw_if(i == 0 && path[pos_in_path]);
        }
    } else {
        throw_if(pos_in_used >= used_codes.size());
        cur->word = Word(used_codes[pos_in_used++]);
    }
    throw_if(pos_in_path >= path.size());
    throw_if(!path[pos_in_path]);
    pos_in_path++;
}


Tree::Tree(std::vector<size_t> const &counts) {
    build_tree(counts);
}


Tree::Tree(Tree &&other) noexcept {
    swap(other);
}


void Tree::swap(Tree &other) {
    std::swap(root, other.root);
    std::swap(current_node, other.current_node);
    std::swap(codes, other.codes);
}

