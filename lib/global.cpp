#include "global.h"
#include <fstream>
#include <iostream>

vector<REC>* init(string filename) {
  vector<REC> *table;
  table = new vector<REC>;
  ifstream inp;
  if (filename == "")
    filename = "data.txt";
  try {
    inp.open(filename);
  } catch (...) {}
  if (inp.is_open()) { cout << "Data found." << endl; }
  else {
    cout << "Failed to find data." << endl;
    return NULL;
  }
  // effort to finding the file of data
  string row = "";
  int cnt = 0;
  while (not inp.eof()) {
    char c = inp.get();
    if ((c == 10) || (c == 13) || (c == -1)) {
      if (row != "") {
        split_and_save(table, row);
        row = "";
      }
    } else {
      row = row + c;
    }
  }
  return table;
}

void split_and_save(vector<REC> *t, string r) {
  string clip = r;
  string val;
  REC record;
  for (int i = 0; i < 15; ++i) {
    val = "";
    int x = clip.find_first_of(',');
    while (clip[x + 1] == ' ') {
      val += clip.substr(0, x + 1);
      clip.erase(0, x + 1);
      x = clip.find_first_of(',');
    }
    val += clip.substr(0, x);
    clip.erase(0, x + 1);
    record.attr[i] = val;
  }
  (*t).push_back(record);
}
