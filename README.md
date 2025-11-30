SYSC4001 Assignment 3 Part 1

This repository includes my work for the EP and RR schedulers for Part 1 of the assignment.
Everything was tested on Linux (WSL Ubuntu 24.04).

1. Folder Structure
SYSC4001_A3P1/
│
├── interrupts_student1_student2_EP.cpp
├── interrupts_student1_student2_RR.cpp
├── interrupts_student1_student2_EP_RR.cpp
├── interrupts_student1_student2.hpp
│
├── input_files/
│   ├── test1.txt
│   ├── test2.txt
│   ├── test3.txt
│   └── test4.txt
│
├── execution.txt
│
└── README.md


All input files are kept in the input_files folder, and each program writes its output to execution.txt.

2. How to Compile and Run

All commands must be run in Linux.

2.1 EP Scheduler

Go into the folder:

cd SYSC4001_A3P1


Compile:

g++ interrupts_student1_student2_EP.cpp -o EP


Run with any test file:

./EP input_files/test1.txt


The program will load the input file and generate execution.txt with all state transitions.

2.2 RR Scheduler

Go into the folder:

cd SYSC4001_A3P1


Compile:

g++ interrupts_student1_student2_RR.cpp -o RR


Run:

./RR input_files/test2.txt


The RR version uses a 100ms quantum and respects priority.

2.3 Combined EP + RR (Optional)

Go into the folder:

cd SYSC4001_A3P1


Compile:

g++ interrupts_student1_student2_EP_RR.cpp -o EP_RR


Run:

./EP_RR input_files/test3.txt

3. Input Format

Each process is defined on one line:

arrival_time, PID, priority, cpu_time, io_frequency, io_duration


Example:

10, 1, 0, 10, 5, 1

4. Output

All transitions (NEW → READY, READY → RUNNING, RUNNING → WAITING, WAITING → READY, TERMINATED, and RR preemptions) are written to:

execution.txt


The format matches the required transition table from the assignment.

5. Notes

Running a new test will overwrite execution.txt.

Input files must be stored in the input_files directory.

All testing was done in WSL on Ubuntu 24.04..04.
