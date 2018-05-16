#include "lvlcompute.h"

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

void compute(int lvl, vector<REC> *table) {
    set<int> *lvl_set = comp_lvl_set(lvl);
    /*
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
}
