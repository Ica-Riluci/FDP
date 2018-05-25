#include "lvlcompute.h"
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;

int part_size[1 << attr_num];
int rhsp[1 << attr_num];
vector<set<int>> p[1 << attr_num];

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
    set<int> *pre_set = new set<int>;
    set<int> *cur_set;
    for (int i = 0; i < (1 << attr_num); ++i) {
        part_size[i] = -1;
        rhsp[i] = -1;
    }
    cout << "Initializing the computation..." << endl;
    for (int i = 0; i < attr_num; ++i) {
        rhsp[1 << i] = (1 << attr_num) - 1;
        pre_set->insert((1 << attr_num) - 1);
        for (vector<REC>::size_type j = 0; j != (*table).size(); ++j) {
            // vector<string> item;
            // item.push_back((*table)[j].attr[i]);
            // cnt->insert(item);
            if (p[1 << i].size() == 0) {
                set<int> new_p;
                new_p.insert(j);
                p[1 << i].push_back(new_p);
            } else {
                vector<set<int>>::iterator k = p[1 << i].begin();
                for (; k != p[1 << i].end(); ++k) {
                    if ((*table)[*((*k).begin())].attr[i] == (*table)[j].attr[i]) {
                        (*k).insert(j);
                        break;
                    }
                }
                if (k == p[1 << i].end()) {
                    set<int> new_p;
                    new_p.insert(j);
                    p[1 << i].push_back(new_p);
                }
            }
        }
        part_size[1 << i] = p[1 << i].size();
    }
    for (int l = 2; l <= attr_num; ++l) {
        cout << "level " << l << " >>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "compute level candidates" << endl;
        cur_set = compute_lvl_set(l);
        int pT[1000000];
        for (int i = 0; i < 1000000; ++i)
            pT[i] = -1;
        for (set<int>::iterator it = cur_set->begin(); it != cur_set->end(); ++it) {
            int tid = (*it);
            int pia = 0, pib;
            // cout << "For " << bitset<attr_num>(tid) << ":" << endl;
            while (pia < attr_num) {
                if ((tid >> pia) & 1)
                    if (p[tid - (1 << pia)].size() > 0)
                        break;
                ++pia;
            }
            if (pia == attr_num)
                continue;
            pib = pia + 1;
            while (pib < attr_num) {
                if ((tid >> pib) & 1)
                    if (p[tid - (1 << pib)].size() > 0)
                        break;
                ++pib;
            }
            if (pib == attr_num)
                continue;
            pia = tid - (1 << pia);
            pib = tid - (1 << pib);
            // cout << "Pi_a : " << bitset<attr_num>(pia) << " & Pi_b : " << bitset<attr_num>(pib) << endl;
            // cout << "Partition of Pi_a:" << endl;
            // cout << "{";
            // for (vector<set<int>>::iterator j = p[pia].begin(); j != p[pia].end(); ++j) {
            //     cout << "{";
            //     for (set<int>::iterator k = (*j).begin(); k != (*j).end(); ++k) {
            //         cout << (*k) << ',';
            //     }
            //     cout << "}";
            // }
            // cout << "}" << endl;
            // cout << "Partition of Pi_b:" << endl;
            // cout << "{";
            // for (vector<set<int>>::iterator j = p[pib].begin(); j != p[pib].end(); ++j) {
            //     cout << "{";
            //     for (set<int>::iterator k = (*j).begin(); k != (*j).end(); ++k) {
            //         cout << (*k) << ',';
            //     }
            //     cout << "}";
            // }
            // cout << "}" << endl;
            set<int> pS[p[pia].size()];
            for (vector<set<int>>::size_type j = 0; j < p[pia].size(); ++j)
                for (set<int>::iterator k = p[pia][j].begin(); k != p[pia][j].end(); ++k) {
                    // cout << "T[" << (*k) << "]=" << j << endl;
                    pT[(*k)] = j;
                }
            for (vector<set<int>>::size_type j = 0; j < p[pib].size(); ++j) {
                for (set<int>::iterator k = p[pib][j].begin(); k != p[pib][j].end(); ++k) {
                    // cout << "T[" << (*k) << "]=" << pT[(*k)] << endl;
                    if (pT[(*k)] != -1)
                        pS[pT[(*k)]].insert((*k));
                }
                for (set<int>::iterator k = p[pib][j].begin(); k != p[pib][j].end(); ++k) {
                    if (pS[pT[(*k)]].size() >= 1)
                        p[tid].push_back(pS[pT[(*k)]]);
                    pS[pT[(*k)]].clear();
                }
            }
            for (int j = 0; j < 1000000; ++j)
                pT[j] = -1;
            part_size[tid] = p[tid].size();
            // cout << "Partition size of " << bitset<attr_num>(tid) << ": " << part_size[tid] << endl;
            //calc pi = pia*pib
            rhsp[tid] = (1 << attr_num) - 1;
            for (int j = 0; j < attr_num; ++j) {
                if ((tid >> j) & 1) {
                    int sonid = tid & ((1 << attr_num) - 1 - (1 << j));
                    if (rhsp[sonid] != -1)
                        rhsp[tid] &= rhsp[sonid];
                }
            }
            int candidates = tid & rhsp[tid];
            for (int j = 0; j < attr_num; ++j) {
                if ((candidates >> j) & 1) {
                    if (part_size[tid & ((1 << attr_num) - 1 - (1 << j))] == part_size[tid]) {
                        cout << "   $$$validated" << endl;
                        ANS tmpans;
                        tmpans.lh = tid & ((1 << attr_num) - 1 - (1 <<j));
                        tmpans.rh = 1 << j;
                        cout << bitset<attr_num>(tmpans.lh) << "->" << bitset<attr_num>(tmpans.rh) << endl;
                        ans.insert(tmpans);
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
            if (rhsp[tid] == 0)
                cur_set->erase(tid);
        }
        delete pre_set;
        pre_set = cur_set;
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
