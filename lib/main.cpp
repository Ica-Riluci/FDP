/*Initialzation
  Here is the pesudo-code
  COMPUTE-DEPENDENCIES(L_l)
    for each X in L_l do
      RHS^+(X) := Interception_(E in X)RHS^+(X \ {E})
      for each E in (X Interception RHS^+(X)) do
        if X \ {E} to E is valid then
          output X \ {E} to E
          remove E from RHS^+(X)
          remove all F in (R \ X) from RHS^+(X)
      if RHS^+(X) is empty do
        delete X from L_l
    Also computes the sets RHS^+(X) for all X in L_l
*/

#include <iostream>
#include <set>
#include <bitset>
#include "global.h"
#include "lvlcompute.h"

using namespace std;

vector<REC> *table;

int main(int argc, char *argv[]) {
    // check input and start initialzation
    if (argc > 1) {
        if (argc == 2)
            table = init(argv[1]);
        else
            cout << "Too many arguments." << endl;
    }
    else
        table = init("");
    // workout
    // set<int> *R = compute_lvl_set(15);
    compute(table);
    // debug
    return 0;
}
