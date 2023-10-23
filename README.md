### How to run the program

> In the directory where the files are present run the following commands

```
Compile command: g++ --std=c++11 SodukuSolver.cpp
Run commands: (different examples)

1. Run without verbose mode
./a.out -d <directory-where-input-file-present>
eg:
./a.out -d /home/as18464/Soduku/

2. Run in verbose mode
./a.out -v -d <directory-where-input-file-present>
eg:
./a.out -v -d /home/as18464/Soduku/

NOTE: 
a. By default, the SodukuSolver looks for an input.txt file in the directory specified in the above commands
b. Please update the contents of input.txt file before every test run
```

After every successful run, the following files are created in the directory specified in commands 1/2:

- cnf.txt: This file contains the generated clauses in CNF form
- dpll.txt: This file contains the final assignments after applying the DPLL algorithm on CNF clauses
- output.txt: This file contains the solved soduku puzzle after converting the output of DPLL to 9*9 matrix

The code was successfully tested on the following department Linux machines:
- https://snappy1.cims.nyu.edu/

NOTE:
The program takes takes a long time for Hard and Expert test cases.
Average time taken for others are as follows:
- Easy: 15 seconds
- Medium: 3 minutes
- Evil: 25 seconds
