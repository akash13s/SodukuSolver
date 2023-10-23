//
// Created by Akash Shrivastva on 10/18/23.
//

#ifndef DPLLSOLVER_DPLLSOLVER_H
#define DPLLSOLVER_DPLLSOLVER_H

#include "set"
#include "vector"
#include "algorithm"
#include "unordered_map"
#include "map"
#include <stdio.h>
#include "string"
#include "iostream"
#include "Helper.h"

using namespace std;

class DpllSolver {
public:
    DpllSolver(set<vector<string>> s, vector<string> atoms, bool isVerbose) {
        this->s = s;
        this->atoms = atoms;
        this->isVerbose = isVerbose;
    }

private:
    set<vector<string>> s;
    vector<string> atoms;
    map<string, int> v;
    bool isVerbose;

    map<string, int> solve(set<vector<string>> se, map<string, int> val) {
        while (1) {
            if (se.empty()) {
                for (int i=0; i<atoms.size(); i++) {
                    if (val[atoms[i]] == -1) {
                        val[atoms[i]] = 1;
                    }
                }
                return val;
            } else {
                vector<vector<string>> clauses = Helper::getClauses(se);

                if (emptyClauseInSet(clauses)) {
                    map<string, int> ma;
                    return ma;
                } else {
                    string pureLiteral = pureLiteralInSet(clauses, val);
                    if (!pureLiteral.empty()) {
                        //  encountered a pure literal
                        if (isVerbose) {
                            cout<<"easy case: pure literal "<<message(pureLiteral)<<endl;
                        }
                        obviousAssignment(pureLiteral, val);
                        // delete every clause containing pure literal from S
                        vector<vector<string>> removeClauses = Helper::getClausesToBeRemoved(clauses, pureLiteral);
                        Helper::deleteClausesFromSet(se, removeClauses);
                    } else {
                        string unitLiteral = unitLiteralInSet(clauses);
                        if (!unitLiteral.empty()) {
                            //  encountered a unit literal
                            if (isVerbose) {
                                cout<<"easy case: unit literal "<<unitLiteral<<endl;
                            }
                            obviousAssignment(unitLiteral, val);
                            propagate(unitLiteral, se, clauses, val);
                        } else {
                            break;
                        }
                    }
                }
            }
        }

        // pick an atom and assign TRUE - hard case
        string atom = getUnboundAtom(val);

        val[atom] = 1;
        set<vector<string>> se1(se);
        vector<vector<string>> clauses1 = Helper::getClauses(se1);
        propagate(atom, se1, clauses1, val);
        if (isVerbose) {
            cout<<"hard case: guess "<<atom<<"=true"<<endl;
        }
        map<string, int> vnew = solve(se1, val);

        if (!vnew.empty()) {
            return vnew;
        }

        cout<<"contradiction: backtrack guess "<<atom<<"=false"<<endl;

        val[atom] = 0;
        set<vector<string>> se2(se);
        vector<vector<string>> clauses2 = Helper::getClauses(se2);
        propagate(Helper::negation(atom), se2, clauses2, val);
        if (isVerbose) {
            cout<<"hard case: guess "<<atom<<"=false"<<endl;
        }
        return solve(se2, val);
    }

    string message(string literal) {
        string msg;
        if (literal[0] == '!') {
            msg = literal.substr(1, literal.length() - 1) + "=false";
        } else {
            msg = literal + "=true";
        }
        return msg;
    }

    string getUnboundAtom(map<string, int> &val) {
        // returns first unbound atom
        for (auto itr = val.begin(); itr!=val.end(); itr++) {
            if (itr->second == -1) {
                return itr->first;
            }
        }

        return "";
    }

    string getUnboundAtomV2(map<string, int> &val) {
        vector< pair<int, string> > vec;

        for (auto itr = val.begin(); itr!=val.end(); itr++) {
            if (itr->second == -1) {
                int i, j;
                string atom = itr->first;
                int v = atom[1];
                int row = atom[4];
                int col = atom[7];

                int count = 0;
                unordered_map<string, int> mark;

                // check for same row
                for (i=1; i<=9; i++) {
                    if (i!=col) {
                        string atom2 = Helper::getAtomForSquare(row, i, v, false);
                        if (val[atom2] == 1 && mark[atom2]==0) {
                            count++;
                            mark[atom2] = 1;
                        }
                    }
                }

                // check for same column
                for (i=1; i<=9; i++) {
                    if (i!=row) {
                        string atom2 = Helper::getAtomForSquare(i, col, v, false);
                        if (val[atom2] == 1 && mark[atom2]==0) {
                            count++;
                            mark[atom2] = 1;
                        }
                    }
                }

                // check for same 3*3 subgrid
                int ri = 3*((row-1)/3) + 1;
                int ci = 3*((col-1)/3) + 1;
                for (i=ri; i<ri+3; i++) {
                    for (j=ci; j<ci+3; j++) {
                        if (i==row and j==col) {
                            continue;
                        }
                        string atom2 = Helper::getAtomForSquare(i, j, v, false);
                        if (val[atom2] == 1 && mark[atom2] == 0) {
                            count++;
                        }
                    }
                }

                vec.push_back({count, atom});

            }
        }

        sort(vec.begin(), vec.end());
        reverse(vec.begin(), vec.end());

        return vec[0].second;
    }

    bool emptyClauseInSet(vector<vector<string>> &clauses) {
        for (int i=0; i<clauses.size(); i++) {
            if (clauses[i].empty()) {
                return true;
            }
        }
        return false;
    }

    // returns pure literal
    string pureLiteralInSet(vector<vector<string>> &clauses, map<string, int> &val) {
        map<string, int> vis;

        for (int i=0; i<clauses.size(); i++) {
            for (int j=0; j<clauses[i].size(); j++) {
                vis[clauses[i][j]] = 1;
            }
        }

        string pureLiteral = "";

        for (auto itr = val.begin(); itr!=val.end(); itr++) {
            if (itr->second == -1) {
                string literal = itr->first;
                string negatedLiteral = Helper::negation(literal);
                if (vis[literal] == 1 && vis[negatedLiteral] == 0){
                    pureLiteral = literal;
                    break;
                } else if (vis[literal] == 0 && vis[negatedLiteral] == 1) {
                    pureLiteral = negatedLiteral;
                    break;
                }
            }
        }

        return pureLiteral;
    }

    string unitLiteralInSet(vector<vector<string>> &clauses) {
        string unitLiteral = "";

        for (int i=0; i<clauses.size(); i++) {
            if (clauses[i].size() == 1) {
                unitLiteral = clauses[i][0];
                break;
            }
        }

        return unitLiteral;
    }

    void obviousAssignment(string literal, map<string, int> &val) {
        if (literal[0] == '!') {
            val[Helper::negation(literal)] = 0;
        } else {
            val[literal] = 1;
        }
    }

    void propagate(string literal, set<vector<string>> &se,
                   vector<vector<string>> clauses, map<string, int> &val) {
        string negatedLiteral = Helper::negation(literal);
        for (int i=0; i<clauses.size(); i++) {
            vector<string> clause = clauses[i];
            bool isPresent = false;
            bool isNegationPresent = false;

            for (int j=0; j<clause.size(); j++) {
                if (clause[j] == literal) {
                    isPresent = true;
                    break;
                } else if (clause[j] == negatedLiteral) {
                    isNegationPresent = true;
                    break;
                }
            }

            if (isPresent) {
                se.erase(clause);
            } else if (isNegationPresent) {
                vector<string> oldClause(clause);
                clause.erase(std::find(clause.begin(), clause.end(), negatedLiteral));
                se.erase(oldClause);
                se.insert(clause);
            }
        }
    }

    void initAtoms() {
        for (int i=0; i<atoms.size(); i++) {
            v[atoms[i]] = -1;
        }
    }

public:
    map<string, int> solve() {
        initAtoms();
        return solve(s, v);
    }
};


#endif //DPLLSOLVER_DPLLSOLVER_H
