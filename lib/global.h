#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
#include <set>

const int attr_num = 15;

using namespace std;

typedef struct rec {
    string attr[attr_num];
} REC;

typedef struct trie {
    trie *son[128];
    set<int> rows;
    trie() {
        for (int i = 0; i < 128; ++i)
            son[i] = NULL;
        rows.clear();
    }
} DICT;

void init(string filename, DICT** table);

void dict_ins(DICT *p, string rest, int row);

void split_and_save(DICT **t, string r, int row);

#endif
