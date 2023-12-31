//
// Created by Akash Shrivastva on 10/16/23.
//

#include "SodukuSolver.h"
#include <iostream>
#include "chrono"

using namespace std;

string inputFile, cnfFile, dpllFile, outputFile;
bool isVerbose = false;
string baseDir;

void readCommandLineArguments(int argc, char *argv[]) {

    for (int i = 0; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-v") {
            isVerbose = true;
        } else if (arg == "-d") {
            if (i+1<argc) {
                baseDir = argv[i+1];
            }
        }
    }
}

int init(int argc, char *argv[]) {

    readCommandLineArguments(argc, argv);

    if (baseDir.empty()) {
        cout<<"Error: no input/output directory found"<<endl;
        return 0;
    }
    if (baseDir[baseDir.length()-1]!='/') {
        baseDir += '/';
    }
    inputFile = baseDir + "input.txt";
    cnfFile = baseDir + "cnf.txt";
    dpllFile = baseDir + "dpll.txt";
    outputFile = baseDir + "output.txt";
    return 1;
}

int main(int argc, char *argv[]) {
    if (!init(argc, argv)) {
        return 1;
    }

    bool correctInputFormat = true;

    vector<string> input = Helper::parseInputFile(inputFile, correctInputFormat);
    if (!correctInputFormat) {
        cout<<"Error: Incorrect input format"<<endl;
        return 1;
    }

    auto *solver = new SodukuSolver(input, isVerbose);
    solver->writeCnfClausesToFile(cnfFile);

    auto start = std::chrono::high_resolution_clock::now();

    map<string, int> valuation = solver->dpll();

    auto end = std::chrono::high_resolution_clock::now();

    if (valuation.empty()) {
        cout<<"No valid assignment found"<<endl;
    }
    cout<<"Success: valid assignments found"<<endl;
    std::chrono::duration<double> duration = end - start;
    cout<<"Time taken(in seconds): "<<duration.count()<<endl;

    solver->writeDpllAssignmentsToFile(dpllFile);
    vector<vector<int>> solvedMatrix = solver->getSolvedPuzzle();
    solver->writeSolvedPuzzleToFile(outputFile, solvedMatrix);
    Helper::printMatrix(solvedMatrix);

}
