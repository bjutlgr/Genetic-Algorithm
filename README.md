ECE3090-Assignment-5-Genetic-Algorithm
======================================
Author: Peihsun (Ben) Yeh
Implements a genetic algorithm for pathfinding

Summary
=======
This problem describes a map containing a number of cans. You control a robot that must pick up all the cans using the least number of actions. 

The problem parameters are:
- A map of size NxM
- An allotted number of actions K
- A starting point on the map (X0,Y0)
- Can coordinates (Xn,Yn)

The problem outputs a path that travels between all the cans once, along with number of actions taken. This output is recorded in "output.dat" in the same directory.

A more detailed problem description can be found in Project5.pdf and Project5Slides.pdf.

Running
=======
Make sure the following files are in the same directory:
- Project5.cpp
- Map.h
- Map.cpp
- input.dat
  - Make sure that input.dat contains one of each of the following lines:
    - MAP N M
    - ACTIONS K
    - START X0 Y0
    - and any number of CAN Xn Yn

To compiile, in terminal, type "make". To run, type "./Project5.o". When the program is done running, it will output "DONE!" to the terminal. 

Implementation details
======================
This problem is realized as a Traveling Salesman Problem, with each can representing a node. TSP is known to be NP-hard, so if the output solution exceeds the maximum number of actions taken, it is possible the algorithm is caught in a local maximum, or the optimal solution does not exist. Even if the output solution does not violate the maximum number of actions allowed, there is no guarantee that the solution is optimal. 

Possible future changes
=======================
- Optimizing and reducing run time
- Output the maximum number of cans that can be collected bounded by the maximum number of cans. 