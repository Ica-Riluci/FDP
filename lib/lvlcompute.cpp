#include "lvlcompute.h"
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;

int rhsp[1 << attr_num];
vector<vector<int>> *p[1 << attr_num];

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

vector<int> to_vector(set<int> s) {
    vector<int> res;
    res.clear();
    for (set<int>::iterator i = s.begin(); i != s.end(); ++i)
        res.push_back((*i));
    return res;
}

vector<vector<int>> cover(DICT *p) {
    vector<vector<int>> res;
    res.clear();
    if (p->rows.size() > 1)
        res.push_back(to_vector(p->rows));
    for (int i = 0; i < 128; ++i)
        if (p->son[i] != NULL) {
            vector<vector<int>> sonres = cover(p->son[i]);
            for (vector<vector<int>>::size_type j = 0; j < sonres.size(); ++j)
                res.push_back(sonres[j]);
        }
    return res;
}

PPAIR calc_pi_pair(int t) {
    int a, b;
    for (a = 0; a < attr_num; ++a)
        if (((t >> a) & 1) && (p[t - (1 << a)] != NULL))
            break;
    for (b = a + 1; b < attr_num; ++b)
        if (((t >> b) & 1) && (p[t - (1 << b)] != NULL))
            break;
    if (a >= attr_num)
        for (a = 0; a < attr_num; ++a)
            if ((t >> a) & 1)
                break;
    if (b >= attr_num)
        for (b = 0; b < attr_num; ++b)
            if (((t >> b) & 1) && (a != b))
                break;
    PPAIR res;
    res.pi_a = t - (1 << a);
    res.pi_b = t - (1 << b);
    return res;
}

vector<vector<int>> calc_part(int pia, int pib) {
    vector<vector<int>> res, a, b;
    int pT[150000];
    for (int i = 0; i < 150000; ++i)
        pT[i] = -1;
    if (p[pia] == NULL) {
        PPAIR pp = calc_pi_pair(pia);
        a = calc_part(pp.pi_a, pp.pi_b);
    } else
        a = *(p[pia]);
    if (p[pib] == NULL) {
        PPAIR pp = calc_pi_pair(pib);
        b = calc_part(pp.pi_a, pp.pi_b);
    } else
        b = *(p[pib]);
    if ((a.size() != 0) && (b.size() != 0)) {
        vector<int> pS[a.size()];
        for (vector<vector<int>>::size_type j = 0; j < a.size(); ++j)
            for (vector<int>::iterator k = a[j].begin(); k != a[j].end(); ++k) {
                pT[(*k)] = j;
            }
        for (vector<vector<int>>::size_type j = 0; j < b.size(); ++j) {
            for (vector<int>::iterator k = b[j].begin(); k != b[j].end(); ++k) {
                if (pT[(*k)] != -1)
                    pS[pT[(*k)]].push_back((*k));
            }
            for (vector<int>::iterator k = b[j].begin(); k != b[j].end(); ++k) {
                if (pT[(*k)] < 0)
                    continue;
                if (pS[pT[(*k)]].size() > 1)
                    res.push_back(pS[pT[(*k)]]);
                pS[pT[(*k)]].clear();
            }
        }
        for (int j = 0; j < 150000; ++j)
            pT[j] = -1;
        return res;
    } else
        return res;
}

int get_part_size(int tid) {
    // cout << "get partition size of " << bitset<attr_num>(tid) << endl;
    if (p[tid] == NULL) {
        PPAIR pp = calc_pi_pair(tid);
        return calc_part(pp.pi_a, pp.pi_b).size();
    } else
        return p[tid]->size();
}

void compute(DICT **dict) {
    ANS a1, a2;
    a1.lh = (1 << 1) + (1 << 5) + (1 << 12);
    a1.rh = 0;
    a2.lh = (1 << 1) + (1 << 5);
    a2.rh = 0;
    cout << (a1 < a2) << ' ' << (a2 < a1) << endl;
    set<ANS> ans;
    set<int> *pre_set = new set<int>;
    set<int> *cur_set;
    for (int i = 0; i < attr_num; ++i)
        p[1 << i] = new vector<vector<int>>;
    for (int i = 0; i < (1 << attr_num); ++i)
        rhsp[i] = -1;
    cout << "Initializing the computation..." << endl;
    for (int i = 0; i < attr_num; ++i) {
        // cout << "column " << i << endl;
        rhsp[1 << i] = (1 << attr_num) - 1;
        pre_set->insert((1 << attr_num) - 1);
        *(p[1 << i]) = cover(dict[i]);
    }
    for (int l = 2; l <= attr_num; ++l) {
        cout << "level " << l << " >>>>>>>>>>>>>>>>>>>>" << endl;
        cur_set = compute_lvl_set(l);
        for (set<int>::iterator it = cur_set->begin(); it != cur_set->end(); ++it) {
            // cout << "for " << bitset<attr_num>(*it) << endl;
            int tid = (*it);
            rhsp[tid] = (1 << attr_num) - 1;
            for (int j = 0; j < attr_num; ++j) {
                if ((tid >> j) & 1) {
                    int sonid = tid & ((1 << attr_num) - 1 - (1 << j));
                    if (rhsp[sonid] != -1)
                        rhsp[tid] &= rhsp[sonid];
                }
            }
            // cout << "   compute RHS+" << endl;
            int candidates = tid & rhsp[tid];
            for (int j = 0; j < attr_num; ++j) {
                if ((candidates >> j) & 1) {
                    if (get_part_size(tid & ((1 << attr_num) - 1 - (1 << j))) == get_part_size(tid)) {
                        // cout << "   $$$validated" << endl;
                        ANS tmpans;
                        tmpans.lh = tid & ((1 << attr_num) - 1 - (1 <<j));
                        tmpans.rh = 1 << j;
                        // cout << bitset<attr_num>(tmpans.lh) << "->" << bitset<attr_num>(tmpans.rh) << endl;
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
        }
        if (l > 2)
            for (set<int>::iterator it = pre_set->begin(); it != pre_set->end(); ++it)
                delete p[(*it)];
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
