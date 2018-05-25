#ifndef LVLCOMPUTE_H
#define LVLCOMPUTE_H
#include "global.h"
#include <set>

typedef struct rhsset {
    int id;
    int rhs;
    bool operator < (const rhsset r) const {
        return id < r.id;
    }
} RHS;

typedef struct anspair {
    int lh;
    int rh;
    bool operator < (const anspair p) const {
        for (int i = 0; i < attr_num; ++i)
            if (((lh >> i) & 1) < ((p.lh >> i) & 1))
                return false;
            else if (((lh >> i) & 1) > ((p.lh >> i) & 1))
                return true;
        if (rh < p.rh)
            return true;
        return false;
    }
} ANS;

int search_rhs(int id, set<RHS> *s);

void compute(vector<REC> *tab);

// bool check_val(int can_a, int b_size, vector<REC> *tab, set<PART> *pp);

set<int>* compute_lvl_set(int lvl);
#endif
