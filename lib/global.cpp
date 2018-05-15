#include "global.h"
#include <fstream>
#include <iostream>

void init(string filename) {
  ifstream inp;
  if (filename == "")
    filename = "data.txt";
  try {
    inp.open(filename);
  } catch (...) {}
  if (inp.is_open()) { cout << "Data found." << endl; }
  else {
    cout << "Failed to find data." << endl;
    return;
  }
  // effort to finding the file of data
  string row = "";
  int cnt = 0;
  while (not inp.eof()) {
    char c = inp.get();
    if ((c == 10) || (c == 13) || (c == -1)) {
      if (row != "") {
        cout << row << endl; // tbc
        row = "";
      }
    } else {
      row = row + c;
    }
  }
}
