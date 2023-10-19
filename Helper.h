//
// Created by Akash Shrivastva on 10/18/23.
//

#ifndef DPLLSOLVER_HELPER_H
#define DPLLSOLVER_HELPER_H

#include "set"
#include "vector"
#include "algorithm"
#include "unordered_map"
#include "map"
#include <stdio.h>
#include "string"
#include "fstream"
#include "sstream"
#include "iostream"

using namespace std;

class Helper {

public:
    Helper(){}

    static string negation(string atom) {
        if (!atom.empty()) {
            if (atom[0] == '!') {
                return atom.substr(1, atom.size()-1);
            } else {
                return "!" + atom;
            }
        }
        return "";
    }

    static string getUnsignedAtom(string atom) {
        string s = "";
        if (!atom.empty()) {
            if (atom[0] == '!') {
                s = atom.substr(1, atom.size() - 1);
            } else {
                s = atom;
            }
        }
        return s;
    }

    static vector<string> parseInputFile(string inputFile) {
        ifstream file(inputFile);
        string line;

        vector<string> atoms;

        while (getline(file, line)) {
            removeLeadingWhitespace(line);

            if (line.empty()) {
                continue;
            }

            vector<string> v = split(line, ' ');

            for (int i=0; i<v.size(); i++) {
                vector<string> w = split(v[i], '=');
                int row = w[0][0] - '0';
                int col = w[0][1] - '0';
                int val = w[1][0] - '0';
                // perform validation for row/col/val
                string atom = getAtomForSquare(row, col, val, false);
                atoms.push_back(atom);
            }
        }

        return atoms;
    }

    static string getAtomForSquare(int row, int col, int val, bool negate) {
        string s = "n" + to_string(val) + "_" + "r" + to_string(row) + "_c" + to_string(col);
        if (negate) {
            s = "!" + s;
        }
        return s;
    }

    static void removeLeadingWhitespace(string &input) {
        input.erase(input.begin(), input.begin() + input.find_first_not_of(" \t"));
    }

    static vector<string> split(string input, char del) {
        stringstream ss(input);
        string token;
        vector<string> tokens;

        while (!ss.eof()) {
            getline(ss, token, del);
            removeLeadingWhitespace(token);
            tokens.push_back(token);
        }

        return tokens;
    }

    static vector<vector<string>> getClausesToBeRemoved(vector<vector<string>> &clauses, string literal) {
        int i, j;
        vector<vector<string>> removeClauses;

        for (i=0; i<clauses.size(); i++) {
            bool flag = false;
            for (j=0; j<clauses[i].size(); j++) {
                if (clauses[i][j] == literal) {
                    flag = true;
                    break;
                }
            }

            if (flag) {
                removeClauses.push_back(clauses[i]);
            }
        }

        return removeClauses;
    }

    static void deleteClausesFromSet(set<vector<string>> &se,
                              vector<vector<string>> removeClauses) {
        for (int i=0; i<removeClauses.size(); i++) {
            se.erase(removeClauses[i]);
        }
    }

    static vector<vector<string>> getClauses(set<vector<string>> &se) {
        vector<vector<string>> clauses;

        for (auto itr = se.begin(); itr!=se.end(); itr++) {
            vector<string> clause = *itr;
            clauses.push_back(clause);
        }

        return clauses;
    }

    static void printMatrix(map<string, int> &m) {
        int matrix[9][9];
        for (auto itr = m.begin(); itr!=m.end(); itr++) {
            if (itr->second == 1) {
                string cell = itr->first;
                vector<string> props = split(cell, '_');
                int val = props[0][1] - '0';
                int row = props[1][1] - '0';
                int col = props[2][1] - '0';
                matrix[row-1][col-1] = val;
            }
        }

        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                cout<<matrix[i][j]<<"  ";
            }
            cout<<endl;
        }
    }
};


#endif //DPLLSOLVER_HELPER_H
