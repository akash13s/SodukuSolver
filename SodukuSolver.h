//
// Created by Akash Shrivastva on 10/16/23.
//

#ifndef DPLLSOLVER_SODUKUSOLVER_H
#define DPLLSOLVER_SODUKUSOLVER_H
#include "set"
#include "vector"
#include "algorithm"
#include "unordered_map"
#include "map"
#include <stdio.h>
#include "string"
#include "iostream"
#include "DpllSolver.h"

using namespace std;

class SodukuSolver {

private:
    set<vector<string>> s;
    vector<string> atoms;

private:
    string getAtomForSquare(int row, int col, int val, bool negate) {
        string s = "n" + to_string(val) + "_" + "r" + to_string(row) + "_c" + to_string(col);
        if (negate) {
            s = "!" + s;
        }
        return s;
    }



    void insertIntoMainSet(string x, string y) {
        vector<string> ss;
        ss.push_back(x);
        ss.push_back(y);
        sort(ss.begin(), ss.end());
        s.insert(ss);
    }

    void generateCnfForSingleDigitInBox() {
        int i, j, k, l;

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                vector<string> ss;
                for (k=1; k<=9; k++) {
                    ss.push_back(getAtomForSquare(i, j, k, false));
                }
                sort(ss.begin(), ss.end());
                s.insert(ss);
            }
        }
    }

    void generateCnfForUniqueRow() {
        int i, j, k, l;

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string x = getAtomForSquare(i, j, k, true);
                    for (l=1; l<=9; l++) {
                        if (l!=j) {
                            string y = getAtomForSquare(i, l, k, true);
                            insertIntoMainSet(x, y);
                        }
                    }
                }
            }
        }
    }

    void generateCnfForUniqueColumn() {
        int i, j, k, l;

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string x = getAtomForSquare(i, j, k, true);
                    for (l=1; l<=9; l++) {
                        if (l!=i) {
                            string y = getAtomForSquare(l, j, k, true);
                            insertIntoMainSet(x, y);
                        }
                    }
                }
            }
        }
    }

    void generateCnfForUniqueThreeCrossThreeSubgrid() {
        int i, j, k, l, m;

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                int ri = 3*((i-1)/3) + 1;
                int ci = 3*((j-1)/3) + 1;
                if (i!=1 and j!=1) {
                    continue;
                }
                for (k=1; k<=9; k++) {
                    string x = getAtomForSquare(i, j, k, true);
                    for (l=ri; l<ri+3; l++) {
                        for (m=ci; m<ci+3; m++) {
                            if (l==i and m==j) {
                                continue;
                            }
                            string y = getAtomForSquare(l, m, k, true);
                            insertIntoMainSet(x, y);
                        }
                    }
                }
            }
        }
    }

    void populateCnfForInput(vector<string> &input) {
        for (int i=0; i<input.size(); i++) {
            vector<string> ss;
            ss.push_back(input[i]);
            s.insert(ss);
        }
    }

    void generateCNFs(vector<string> &input) {
        generateCnfForSingleDigitInBox();
        generateCnfForUniqueRow();
        generateCnfForUniqueColumn();
        generateCnfForUniqueThreeCrossThreeSubgrid();
        populateCnfForInput(input);
    }

    void generateAtoms() {
        int i, j, k;
        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string atom = getAtomForSquare(i, j, k, false);
                    atoms.push_back(atom);
                }
            }
        }
    }

public:
    void printClauses() {
        for (auto itr=s.begin(); itr!=s.end(); itr++) {
            vector<string> vv = *itr;
            for (int i=0; i<vv.size(); i++) {
                cout<<vv[i]<<" ";
            }
            cout<<endl;
        }
    }

    SodukuSolver(vector<string> input) {
        generateAtoms();
        generateCNFs(input);
    }

    map<string, int> dpll() {
        auto *dpllSolver = new DpllSolver(s, atoms);
        map<string, int> m = dpllSolver->solve();
        return m;
    }

};


#endif //DPLLSOLVER_SODUKUSOLVER_H