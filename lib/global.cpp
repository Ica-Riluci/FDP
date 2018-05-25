#include "global.h"
#include <fstream>
#include <iostream>

// designed and tested by Ica
// for intialization

void dict_ins(DICT *p, string rest, int row) {
    if (rest == "")
        p->rows.insert(row);
    else {
        if (p->son[(int)rest[0]] == NULL)
            p->son[(int)rest[0]] = new DICT;
        dict_ins(p->son[(int)rest[0]], rest.substr(1), row);
    }
}

void init(string filename, DICT** table) {
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
                split_and_save(table, row, cnt);
                ++cnt;
                row = "";
            }
        } else {
            row = row + c;
        }
    }
}

void split_and_save(DICT **t, string r, int row) {
    string clip = r;
    string val;
    for (int i = 0; i < attr_num; ++i) {
        val = "";
        int x = clip.find_first_of(',');
        while (clip[x + 1] == ' ') {
            val += clip.substr(0, x + 1);
            clip.erase(0, x + 1);
            x = clip.find_first_of(',');
        }
        val += clip.substr(0, x);
        clip.erase(0, x + 1);
        dict_ins(t[i], val, row);
    }
}
