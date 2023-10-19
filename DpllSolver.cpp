//
// Created by Akash Shrivastva on 10/18/23.
//

#include "DpllSolver.h"

using namespace std;

int main111() {

    // example 1
//    vector<string> s1 = {"!M", "N", "P", "!Q"};
//    vector<string> s2 = {"!P", "M"};
//    vector<string> s3 = {"!P", "!N"};
//    vector<string> s4 = {"Q", "M"};
//    vector<string> s5 = {"Q", "!N"};
//    vector<string> s6 = {"M", "!N"};
//    vector<string> s7 = {"!Q", "!N"};
//    vector<string> s8 = {"!Q", "!P"};
//    vector<string> s9 = {"!P", "!Q", "!N"};
//
//    set<vector<string>> s = {s1, s2, s3, s4, s5, s6, s7, s8, s9};
//    vector<string> atoms = {"M", "N", "P", "Q"};

    // example 2
    vector<string> s1 = {"A", "B"};
    vector<string> s2 = {"!A", "C", "D"};
    vector<string> s3 = {"!A", "!D", "C"};
    vector<string> s4 = {"!A", "D", "E"};
    vector<string> s5 = {"!B", "D", "E"};
    vector<string> s6 = {"!C", "!D"};
    vector<string> s7 = {"!C", "E"};
    vector<string> s8 = {"D", "E"};
    vector<string> s9 = {"!E", "D"};

    set<vector<string>> s = {s1, s2, s3, s4, s5, s6, s7, s8, s9};
    vector<string> atoms = {"A", "B", "C", "D", "E"};

    // example 3
//    vector<string> s1 = {"!a", "b", "c"};
//    vector<string> s2 = {"a", "c", "d"};
//    vector<string> s3 = {"a", "c", "!d"};
//    vector<string> s4 = {"a", "!c", "d"};
//    vector<string> s5 = {"a", "!c", "!d"};
//    vector<string> s6 = {"!b", "!c", "d"};
//    vector<string> s7 = {"!a", "b", "!c"};
//    vector<string> s8 = {"!a", "!b", "c"};
//    set<vector<string>> s = {s1, s2, s3, s4, s5, s6, s7, s8};
//    vector<string> atoms = {"a", "b", "c", "d"};

    auto *solver = new DpllSolver(s, atoms);
    map<string, int> valuation = solver->solve();
    if (!valuation.empty()) {
        cout<<"success"<<endl;
    } else {
        cout<<"failure"<<endl;
    }

    return 0;
}