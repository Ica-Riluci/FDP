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
        int i = 0;
        for (; i < attr_num && (((lh >> i) & 1) == ((p.lh >> i) & 1)); ++i);
        if (i < attr_num) {
            if ((lh >> i) & 1) {
                int j = i + 1;
                for (; j < attr_num; ++j)
                    if ((p.lh >> j) & 1 == 1)
                        return true;
                return false;
            } else if ((p.lh >> i) & 1) {
                int j = i + 1;
                for (; j < attr_num; ++j)
                    if ((lh >> j) & 1 == 1)
                        return false;
                return true;
            }
        }
        if (rh < p.rh)
            return true;
        return false;
    }
} ANS;

typedef struct pipair {
    int pi_a, pi_b;
} PPAIR;

int search_rhs(int id, set<RHS> *s);

void compute(DICT **tab);

vector<vector<int>> cover(DICT *p);

// bool check_val(int can_a, int b_size, vector<REC> *tab, set<PART> *pp);

set<int>* compute_lvl_set(int lvl);

vector<int> to_vector(set<int> s);

vector<vector<int>> calc_part(int pia, int pib);

int get_part_size(int tid);

#endif
