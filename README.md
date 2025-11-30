SYSC4001 Assignment 3 – Part 1

This repository contains my implementation of Part 1, including:

Event-Driven Priority Scheduler (EP)

Round Robin Priority Scheduler (RR)

All development and testing were done on WSL Ubuntu 24.04.

📂 1. Folder Structure
Assignment_3/
│
├── interrupts_student1_student2_EP.cpp
├── interrupts_student1_student2_RR.cpp
├── interrupts_student1_student2_EP_RR.cpp   # optional combined version
│
├── interrupts_student1_student2.hpp
│
├── input_files/
│   ├── test1.txt
│   ├── test2.txt
│   ├── test3.txt
│   └── test4.txt
│
├── execution.txt        # auto-generated output
│
└── README.md

⚙️ 2. How to Compile and Run (No Makefile)

All commands must be executed inside Linux / WSL.

📘 2.1 Event-Driven Priority Scheduler (EP)
g++ interrupts_student1_student2_EP.cpp -o EP
./EP input_files/test1.txt


The simulator logs all transitions to:

execution.txt

📙 2.2 Round Robin Priority Scheduler (RR)
g++ interrupts_student1_student2_RR.cpp -o RR
./RR input_files/test2.txt


Uses quantum = 100ms

Still respects priority ordering

📗 2.3 Combined EP + RR Version (Optional)
g++ interrupts_student1_student2_EP_RR.cpp -o EP_RR
./EP_RR input_files/test3.txt

📥 3. Input Format

Each line in the input file defines a process:

arrival_time, PID, priority, cpu_time, io_frequency, io_duration


Example:

10, 1, 0, 10, 5, 1

📤 4. Output Format

All state transitions are written to execution.txt in a table:

+------------------------------------------------+
|Time of Transition |PID | Old State | New State |
+------------------------------------------------+
|                 0 | 10 | NEW       | READY     |
|                 0 | 10 | READY     | RUNNING   |
|                 5 | 10 | RUNNING   | WAITING   |
|                 6 | 10 | WAITING   | READY     |
|                 6 | 10 | READY     | RUNNING   |
|                11 | 10 | RUNNING   | TERMINATED|
+------------------------------------------------+


Transitions include:

NEW → READY

READY → RUNNING

RUNNING → WAITING

WAITING → READY

RUNNING → TERMINATED

RUNNING → READY (RR preemption)

🧠 5. Design Summary
5.1 Event-Driven Priority Scheduler (EP)

No preemption

Highest-priority READY process always chosen

Handles arrivals, I/O events, and termination

Time-driven simulation (1ms increments)

Timestamps adjusted according to assignment spec

5.2 Round Robin Priority Scheduler (RR)

Quantum = 100ms

Preempts on:

quantum expiry

arrival of a higher-priority process

Maintains READY queue ordered by priority

Prevents starvation and ensures fairness

🧪 6. Test Cases

EP:

./EP input_files/test1.txt
./EP input_files/test2.txt
./EP input_files/test3.txt
./EP input_files/test4.txt


RR:

./RR input_files/test1.txt
./RR input_files/test2.txt
./RR input_files/test3.txt
./RR input_files/test4.txt


Outputs were verified manually based on expected scheduler behavior.

📝 7. Notes

Each run overwrites execution.txt.

Ensure .txt files use LF line endings.

All code tested on WSL Ubuntu 24.04.
