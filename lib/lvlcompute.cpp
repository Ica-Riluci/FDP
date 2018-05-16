#include "lvlcompute.h"

/*Initialzation
  Here is the pesudo-code
  COMPUTE-DEPENDENCIES(L_l)
    for each X in L_l do
      RHS^+(X) := Interception_(E in X)RHS^+(X \ {E}) (*)
      for each E in (X Interception RHS^+(X)) do
        if X \ {E} to E is valid then
          output X \ {E} to E
          remove E from RHS^+(X)
          remove all F in (R \ X) from RHS^+(X) (**)
      if RHS^+(X) is empty do
        delete X from L_l (***)
    Also computes the sets RHS^+(X) for all X in L_l
*/

int search_rhs(int qid, set<RHS> *s) {
    set<RHS>::iterator it;
    for (it = s->begin(); it != s->end(); ++it)
        if ((*it).id == qid)
            return (*it).rhs;
    return -1;
}

set<RHS>* compute(vector<int> *table) {
    set<RHS> *pre_lvl = new set<RHS>;
    set<int> *pre_set = new set<int>;
    set<RHS> *cur_lvl = new set<RHS>;
    set<int> *cur_set = new set<int>;
    //calc the RHS+ of lvl_1
    for (int i = 0; i < 15; ++i) {
        RHS tmp;
        tmp.id = 1 << i;
        tmp.rhs = 1 << i;
        pre_lvl->insert(tmp);
        pre_set->insert(1 << i);
    }
    //calc the RHS+ recursively
    for (int l = 2; i <= 15; ++l) {
        cur_set = compute_lvl_set(l);
        set<int>::iterator it;
        for (it = cur_set->begin(); it != cur_set->end(); ++it) {
            RHS tmp;
            tmp.id = *it;
            tmp.rhs = (1 << 15) - 1;
            for (int j = 0; j < 15; ++j) {
                if ((tmp.id >> j) & 1) {
                    sonid = tmp.id & ((1 << 15) - 1 - (1 << j))
                    if (pre_set->find(sonid) != pre_set->end()) {
                        int tmprhs = search_rhs(sonid, pre_lvl);
                        if (tmprhs == -1)
                            return NULL;
                        else
                        tmp.rhs &= tmprhs;
                    }
                }
            }
            // (*)
            int candidates = tmp.id & tmp.rhs;
            for (int j = 0; j < 15; ++j) {
                if ((candidates >> j) & 1) {
                    if (check_val(x & ((1 << 15) - 1 - (1 << j)), 1 << j)) {
                        print_out(x & ((1 << 15) - 1 - (1 << j)), 1 << j);
                        // check_val and print_out need extra work
                        tmp.rhs &= ((1 << 15) - 1 - (1 << j));
                        for (int k = 0; k < 15; ++j) {
                            if (((((1 << 15) - 1) ^ tmp.id) >> k) & 1) {
                                tmp.rhs &= ((1 << 15) - 1 - (1 << j));
                            }
                        }
                    }
                }
            }
            if (tmp.rhs == 0)
                cur_set.erase(tmp.id);
            else
                cur_lvl.insert(tmp);
        }
        delete pre_lvl;
        delete pre_set;
        pre_lvl = cur_lvl;
        pre_set = cur_set;
    }
}
