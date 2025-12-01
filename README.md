SYSC4001 Assignment 3 Part 1
This repository includes my work for the EP scheduler, the RR scheduler, and the optional combined version.
Everything was tested on Linux (WSL Ubuntu 24.04).


How to Compile and Run

All commands must be run in Linux.

2.1 EP Scheduler

Go into the folder:
cd SYSC4001_A3P1

Compile:
g++ interrupts_student1_student2_EP.cpp -o EP

Run with any test file:
./EP input_files/test1.txt

The program will generate execution.txt with all state transitions.

2.2 RR Scheduler

Go into the folder:
cd SYSC4001_A3P1

Compile:
g++ interrupts_student1_student2_RR.cpp -o RR

Run:
./RR input_files/test2.txt

The RR scheduler uses a 100ms quantum and handles priority and preemption.

2.3 Combined EP + RR (Optional)

Go into the folder:
cd SYSC4001_A3P1

Compile:
g++ interrupts_student1_student2_EP_RR.cpp -o EP_RR

Run:
./EP_RR input_files/test3.txt

Input Format

Each process is defined on one line:

arrival_time, PID, priority, cpu_time, io_frequency, io_duration

Notes

Running a new test overwrites execution.txt

Input files must be placed in the input_files directory

All testing was done on WSL Ubuntu 24.04
