#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>

const int attr_num = 15;

using namespace std;

typedef struct rec {
    string attr[attr_num];
} REC;

vector<REC>* init(string filename);

void split_and_save(vector<REC> *t, string r);

#endif
