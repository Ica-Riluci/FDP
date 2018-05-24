#include "lvlcompute.h"
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;

int part_size[1 << attr_num];
int rhsp[1 << attr_num];

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
    for (set<RHS>::iterator it = s->begin(); it != s->end(); ++it) {
        if ((*it).id == qid)
            return (*it).rhs;
    }
    return -1;
}

void compute(vector<REC> *table) {
    set<ANS> ans;
    // set<RHS> *pre_lvl = new set<RHS>;
    // set<PART> *pre_part = new set<PART>;
    set<int> *pre_set = new set<int>;
    // set<RHS> *cur_lvl;
    // set<PART> *cur_part;
    set<int> *cur_set;
    set<vector<string>> *cnt = new set<vector<string>>;
    for (int i = 0; i < (1 << attr_num); ++i) {
        part_size[i] = -1;
        rhsp[i] = -1;
    }
    //calc the RHS+ of lvl_1, tested
    cout << "Initializing the computation..." << endl;
    for (int i = 0; i < attr_num; ++i) {
        // RHS tmp;
        // tmp.id = 1 << i;
        // tmp.rhs = 1 << i;
        // pre_lvl->insert(tmp);
        rhsp[1 << i] = (1 << attr_num) - 1;
        pre_set->insert((1 << attr_num) - 1);
        for (vector<REC>::size_type j = 0; j != (*table).size(); ++j) {
            vector<string> item;
            item.push_back((*table)[j].attr[i]);
            cnt->insert(item);
        }
        // PART tmp_p;
        // tmp_p.id = 1 << i;
        // tmp_p.size = cnt->size();
        // pre_part->insert(tmp_p);
        part_size[1 << i] = cnt->size();
        cnt->clear();
    }
    //calc the RHS+ recursively
    for (int l = 2; l <= attr_num; ++l) {
        cout << "level " << l << " >>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "compute level candidates" << endl;
        cur_set = compute_lvl_set(l);
        // cur_part = new set<PART>;
        // cur_lvl = new set<RHS>;
        for (set<int>::iterator it = cur_set->begin(); it != cur_set->end(); ++it) {
            int tid = (*it);
            rhsp[tid] = (1 << attr_num) - 1;
            // RHS tmp;
            // tmp.id = *it;
            // tmp.rhs = (1 << attr_num) - 1;
            // cout << "for <" << bitset<attr_num>(tid) << ">" << endl;
            for (int j = 0; j < attr_num; ++j) {
                if ((tid >> j) & 1) {
                    int sonid = tid & ((1 << attr_num) - 1 - (1 << j));
                    // cout << "   rhc:" << sonid << endl;
                    // cout << "   rhsp of rhc:" << rhsp[sonid] << endl;
                    if (rhsp[sonid] != -1)
                        rhsp[tid] &= rhsp[sonid];
                }
            }
            // cout << "   compute RHS+" << endl;
            // (*)
            int candidates = tid & rhsp[tid];
            // cout << "   RHS+ candidates = " << candidates << endl;
            // PART new_part;
            // new_part.id = tmp.id;
            for (vector<REC>::size_type z = 0; z != table->size(); ++z) {
                vector<string> ntmp;
                for (int y = 0; y < attr_num; ++y)
                    if ((tid >> y) & 1)
                        ntmp.push_back((*table)[z].attr[y]);
                cnt->insert(ntmp);
            }
            // new_part.size = cnt->size();
            // cur_part->insert(new_part);
            part_size[tid] = cnt->size();
            cnt->clear();
            for (int j = 0; j < attr_num; ++j) {
                if ((candidates >> j) & 1) {
                    // cout << "   validate <" << bitset<attr_num>(tid) << "," << bitset<attr_num>(1 << j) << ">" << endl;
                    // cout << "   checking " << bitset<attr_num>(tid & ((1 << attr_num) - 1 - (1 << j))) << " and " << bitset<attr_num>(tid) << endl;
                    if (part_size[tid & ((1 << attr_num) - 1 - (1 << j))] == part_size[tid]) {
                        cout << "   $$$validated" << endl;
                        ANS tmpans;
                        tmpans.lh = tid & ((1 << attr_num) - 1 - (1 <<j));
                        tmpans.rh = 1 << j;
                        cout << bitset<attr_num>(tmpans.lh) << "->" << bitset<attr_num>(tmpans.rh) << endl;
                        ans.insert(tmpans);
                        // check_val and print_out need extra work
                        rhsp[tid] &= ((1 << attr_num) - 1 - (1 << j));
                        for (int k = 0; k < attr_num; ++k) {
                            if (((((1 << attr_num) - 1) ^ tid) >> k) & 1) {
                                ANS canans;
                                canans.lh = tid & ((1 << attr_num) - 1 - (1 << j)) & ((1 << attr_num) - 1 - (1 << k));
                                canans.rh = 1 << j;
                                if (ans.find(canans) != ans.end())
                                    rhsp[tid] &= ((1 << attr_num) - 1 - (1 << k));
                            }
                        }
                    }
                }
            }
            // cout << "   update level candidates" << endl;
            if (rhsp[tid] == 0)
                cur_set->erase(tid);
        }
        // delete pre_lvl;
        delete pre_set;
        // delete pre_part;
        // pre_lvl = cur_lvl;
        pre_set = cur_set;
        // pre_part = cur_part;
    }
    cout << "Analysis finished." << endl;
    ofstream outp;
    outp.open("result.txt");
    for (set<ANS>::iterator ansit = ans.begin(); ansit != ans.end(); ++ansit) {
        for (int x = 0; x < attr_num; ++x)
            if ((((*ansit).lh) >> x) & 1)
                outp << x + 1 << ' ';
        outp << "-> ";
        for (int x = 0; x < attr_num; ++x)
            if ((((*ansit).rh) >> x) & 1) {
                outp << x + 1 << endl;
                break;
            }
    }
    outp.close();
}

// bool check_val(int can_a, int b_size, vector<REC> *tab, set<PART> *pp) {
    // for (set<PART>::iterator it = pp->begin(); it != pp->end(); ++it)
        // if ((*it).id == can_a) {
            // cout << "   partition size of " << bitset<attr_num>(can_a) << ": " << (*it).size << ' ' << b_size << endl;
            // if ((*it).size == b_size)
                // return true;
        // }
    // return false;
// }

// designed by GML, tested by Ica
// intended to work out the combination of attributes on each level
set<int>* compute_lvl_set(int lvl) {
    set<int>* res = new set<int>();
    if (lvl == 0) {
        res->insert(0);
        return res;
    }
    int *count = new int[32767];
    for (int i = 0; i < attr_num; i++) {
        count[1 << i] = i;
    }
    int current_1 = 0;
    int max_n = (1 << attr_num);
    for (int i = 1; i < max_n; i++) {
        int lowbit = (i & -i);
        current_1 -= count[lowbit] - 1;
        if (current_1 == lvl) {
            res->insert(i);
        }
    }
    delete[] count;
    return res;
}
