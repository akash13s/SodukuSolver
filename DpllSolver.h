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

    map<string, int> solve(set<vector<string>> se) {
        while (1) {
            if (se.empty()) {
                for (int i=0; i<atoms.size(); i++) {
                    if (v[atoms[i]] == -1) {
                        v[atoms[i]] = 1;
                    }
                }
                return v;
            } else {
                vector<vector<string>> clauses = getClauses(se);
                // 1. check for empty clause in set
                // 2. check for pure literal - currently it returns the 1st such literal found
                // 3. check for unit clause
                if (emptyClauseInSet(clauses)) {
                    map<string, int> ma;
                    return ma;
                } else {
                    string pureLiteral = pureLiteralInSet(clauses);
                    if (!pureLiteral.empty()) {
                        //  encountered a pure literal
                        obviousAssign(pureLiteral);
                        // delete every clause containing pure literal from S
                        vector<vector<string>> removeClauses = getClausesToBeRemoved(clauses, pureLiteral);
                        deleteClausesFromSet(se, removeClauses);
                    } else {
                        string unitLiteral = unitLiteralInSet(clauses);
                        if (!unitLiteral.empty()) {
                            //  encountered a unit literal
                            obviousAssign(unitLiteral);
                            propagate(unitLiteral, se, clauses);
                        } else {
                            cout<<"Exiting loop for solve"<<endl;
                            break;
                        }
                    }
                }
            }
        }

        // pick an atom and assign TRUE - hard case
        string atom = getUnboundAtom();

        v[atom] = 1;
        set<vector<string>> se1(se);
        vector<vector<string>> clauses1 = getClauses(se1);
        propagate(atom, se1, clauses1);
        map<string, int> vnew = solve(se1);

        if (!vnew.empty()) {
            return vnew;
        }

        v[atom] = 0;
        set<vector<string>> se2(se);
        vector<vector<string>> clauses2 = getClauses(se2);
        propagate(atom, se2, clauses2);
        return solve(se2);
    }

    string getUnboundAtom() {
        // returns first unbound atom
        for (auto itr = v.begin(); itr!=v.end(); itr++) {
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

    vector<vector<string>> getClauses(set<vector<string>> &se) {
        vector<vector<string>> clauses;

        for (auto itr = se.begin(); itr!=se.end(); itr++) {
            vector<string> clause = *itr;
            clauses.push_back(clause);
        }

        return clauses;
    }

    // returns pure literal
    string pureLiteralInSet(vector<vector<string>> &clauses) {
        map<string, int> vis;

        for (int i=0; i<clauses.size(); i++) {
            for (int j=0; j<clauses[i].size(); j++) {
                vis[clauses[i][j]] = 1;
            }
        }

        string pureLiteral = "";

        for (int i=0; i<atoms.size(); i++) {
            string negatedLiteral = Helper::negation(atoms[i]);
            if (vis[atoms[i]] == 1 && vis[negatedLiteral] == 0){
                pureLiteral = atoms[i];
                break;
            } else if (vis[atoms[i]] == 0 && vis[negatedLiteral] == 1) {
                pureLiteral = negatedLiteral;
                break;
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

    void obviousAssign(string literal) {
        if (literal[0] == '!') {
            v[Helper::negation(literal)] = 0;
        } else {
            v[literal] = 1;
        }
    }

    vector<vector<string>> getClausesToBeRemoved(vector<vector<string>> &clauses, string literal) {
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

    void deleteClausesFromSet(set<vector<string>> &se,
                              vector<vector<string>> &removeClauses) {
        for (int i=0; i<removeClauses.size(); i++) {
            se.erase(removeClauses[i]);
        }
    }

    void propagate(string literal, set<vector<string>> &se,
                   vector<vector<string>> &clauses) {
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
            string atom = Helper::getUnsignedAtom(literal);
            if ((isPresent && v[atom]==1) || (isNegationPresent && v[atom]==0)) {
                se.erase(clause);
            } else if (isPresent && v[atom]==0) {
                vector<string> oldClause(clause);
                clause.erase(std::find(clause.begin(), clause.end(),literal));
                se.erase(oldClause);
                se.insert(clause);
            } else if (isNegationPresent && v[atom]==1) {
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
        return solve(s);
    }
};


#endif //DPLLSOLVER_DPLLSOLVER_H
