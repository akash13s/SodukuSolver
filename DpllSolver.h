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
    DpllSolver(set<vector<string>> s, vector<string> atoms) {
        this->s = s;
        this->atoms = atoms;
    }

private:
    set<vector<string>> s;
    vector<string> atoms;
    map<string, int> v;

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
                        obviousAssignment(pureLiteral, val);
                        // delete every clause containing pure literal from S
                        vector<vector<string>> removeClauses = Helper::getClausesToBeRemoved(clauses, pureLiteral);
                        cout<<"remove clauses containing pure literal: "<<pureLiteral<<endl;
                        Helper::deleteClausesFromSet(se, removeClauses);
                    } else {
                        string unitLiteral = unitLiteralInSet(clauses);
                        if (!unitLiteral.empty()) {
                            //  encountered a unit literal
                            obviousAssignment(unitLiteral, val);
                            propagate(unitLiteral, se, clauses, val);
                        } else {
                            cout<<"Exiting while loop"<<endl;
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
        map<string, int> vnew = solve(se1, val);

        if (!vnew.empty()) {
            return vnew;
        }

        val[atom] = 0;
        set<vector<string>> se2(se);
        vector<vector<string>> clauses2 = Helper::getClauses(se2);
        propagate(Helper::negation(atom), se2, clauses2, val);
        return solve(se2, val);
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
