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
};


#endif //DPLLSOLVER_HELPER_H
