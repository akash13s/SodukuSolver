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
    map<string, int> assignments;
    bool isVerbose;

private:
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
                    ss.push_back(Helper::getAtomForSquare(i, j, k, false));
                }
                sort(ss.begin(), ss.end());
                s.insert(ss);
            }
        }

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string x = Helper::getAtomForSquare(i, j, k, true);
                    for (l=1; l<=9; l++) {
                        if (l!=k) {
                            string y = Helper::getAtomForSquare(i, j, l, true);
                            insertIntoMainSet(x, y);
                        }
                    }
                }
            }
        }
    }

    void generateCnfForUniqueRow() {
        int i, j, k, l;

        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string x = Helper::getAtomForSquare(i, j, k, true);
                    for (l=1; l<=9; l++) {
                        if (l!=j) {
                            string y = Helper::getAtomForSquare(i, l, k, true);
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
                    string x = Helper::getAtomForSquare(i, j, k, true);
                    for (l=1; l<=9; l++) {
                        if (l!=i) {
                            string y = Helper::getAtomForSquare(l, j, k, true);
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
                for (k=1; k<=9; k++) {
                    string x = Helper::getAtomForSquare(i, j, k, true);
                    for (l=ri; l<ri+3; l++) {
                        for (m=ci; m<ci+3; m++) {
                            if (l==i and m==j) {
                                continue;
                            }
                            string y = Helper::getAtomForSquare(l, m, k, true);
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

    void generateAllAtoms() {
        int i, j, k;
        for (i=1; i<=9; i++) {
            for (j=1; j<=9; j++) {
                for (k=1; k<=9; k++) {
                    string atom = Helper::getAtomForSquare(i, j, k, false);
                    atoms.push_back(atom);
                }
            }
        }
    }

public:
    void writeCnfClausesToFile(string cnfFile) {
        ofstream outputFile;
        outputFile.open(cnfFile);

        if (!outputFile.is_open()) {
            cerr << "Error creating the CNF file." << endl;
        }

        for (auto itr=s.begin(); itr!=s.end(); itr++) {
            vector<string> vv = *itr;
            for (int i=0; i<vv.size(); i++) {
                outputFile<<vv[i]<<" ";
            }
            outputFile<<endl;
        }

        outputFile.close();
    }

    void writeDpllAssignmentsToFile(string dpllFile) {
        ofstream outputFile;
        outputFile.open(dpllFile);

        if (!outputFile.is_open()) {
            cerr << "Error creating the DPLL assignments file." << endl;
        }

        if (!assignments.empty()) {
            for (auto itr = assignments.begin(); itr!=assignments.end(); itr++) {
                string literal = itr->first;
                string assignment = (itr->second == 1)? "true": "false";
                outputFile<<literal<<" = "<<assignment<<endl;
            }
        }

        outputFile.close();
    }

    void writeSolvedPuzzleToFile(string &solvedPuzzleFile, vector<vector<int>> &matrix) {
        ofstream outputFile;
        outputFile.open(solvedPuzzleFile);

        if (!outputFile.is_open()) {
            cerr << "Error creating the DPLL assignments file." << endl;
        }

        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                outputFile<<matrix[i][j]<<"  ";
            }
            outputFile<<endl;
        }

        outputFile.close();
    }

    vector<vector<int>> getSolvedPuzzle() {
        vector<vector<int>> matrix(9, vector<int>(9, 0));

        for (auto itr = assignments.begin(); itr!=assignments.end(); itr++) {
            if (itr->second == 1) {
                string cell = itr->first;
                vector<string> props = Helper::split(cell, '_');
                int val = props[0][1] - '0';
                int row = props[1][1] - '0';
                int col = props[2][1] - '0';
                matrix[row-1][col-1] = val;
            }
        }

        return matrix;
    }

    SodukuSolver(vector<string> input, bool isVerbose) {
        generateAllAtoms();
        generateCNFs(input);
        this->isVerbose = isVerbose;
    }

    map<string, int> dpll() {
        auto *dpllSolver = new DpllSolver(s, atoms, isVerbose);
        assignments = dpllSolver->solve();
        return assignments;
    }

};


#endif //DPLLSOLVER_SODUKUSOLVER_H
