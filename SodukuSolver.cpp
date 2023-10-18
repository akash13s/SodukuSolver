//
// Created by Akash Shrivastva on 10/16/23.
//

#include "SodukuSolver.h"
#include <iostream>

using namespace std;

vector<string> generateDummyInput() {
    vector<string> input;

    // row 1
    input.push_back("n4_r1_c1");
    input.push_back("n2_r1_c2");
    input.push_back("n8_r1_c4");
    input.push_back("n7_r1_c5");
    input.push_back("n5_r1_c6");
    input.push_back("n3_r1_c7");
    input.push_back("n6_r1_c9");

    // row 2
    input.push_back("n3_r2_c1");
    input.push_back("n9_r2_c2");
    input.push_back("n8_r2_c3");
    input.push_back("n4_r2_c5");
    input.push_back("n6_r2_c6");
    input.push_back("n7_r2_c7");
    input.push_back("n1_r2_c9");

    // row 3
    input.push_back("n5_r3_c3");
    input.push_back("n8_r3_c7");
    input.push_back("n2_r3_c9");

    // row 4
    input.push_back("n9_r4_c1");
    input.push_back("n6_r4_c3");
    input.push_back("n4_r4_c4");
    input.push_back("n5_r4_c5");

    // row 5
    input.push_back("n1_r5_c6");
    input.push_back("n8_r5_c8");

    // row 6
    input.push_back("n1_r6_c1");
    input.push_back("n4_r6_c2");
    input.push_back("n6_r6_c7");

    // row 7
    input.push_back("n2_r7_c5");
    input.push_back("n4_r7_c6");
    input.push_back("n5_r7_c7");
    input.push_back("n3_r7_c8");

    // row 8
    input.push_back("n3_r8_c2");
    input.push_back("n1_r8_c4");
    input.push_back("n8_r8_c5");
    input.push_back("n6_r8_c8");

    // row 9
    input.push_back("n9_r9_c3");
    input.push_back("n4_r9_c7");

    return input;
}

int main1() {

    vector<string> input = generateDummyInput();

    auto *solver = new SodukuSolver(input);

    solver->printClauses();

    map<string, int> valuation = solver->dpll();

    int x;

}
