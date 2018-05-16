#ifndef LVLCOMPUTE_H
#define LVLCOMPUTE_H
#include "global.h"

typedef struct rhsset {
    int id;
    int rhs;
    bool operator < (const RHS r) const {
        return id < r.id;
    }
} RHS;

void compute(int lvl, vector<int> *tab);

set<int>* compute_lvl_set(int lvl);
#endif
