#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>

using namespace std;

typedef struct rec {
    string attr[15];
} REC;

vector<REC>* init(string filename);

void split_and_save(vector<REC> *t, string r);

#endif
