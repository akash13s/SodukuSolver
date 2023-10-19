//
// Created by Akash Shrivastva on 10/16/23.
//

#include "SodukuSolver.h"
#include <iostream>

using namespace std;

int main() {

    string inputFile = "/Users/akashshrivastva/Documents/DpllSolver/input.txt";

    vector<string> input = Helper::parseInputFile(inputFile);

    auto *solver = new SodukuSolver(input);
//    solver->printClauses();
    map<string, int> valuation = solver->dpll();
    if (valuation.empty()) {
        cout<<"failure"<<endl;
    } else {
        cout<<"success"<<endl;
        Helper::printMatrix(valuation);
    }

    int x;

}
