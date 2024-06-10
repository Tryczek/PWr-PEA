
# README.md

## Overview

This repository encompasses solutions to problems from the "Efficient Algorithm Design" course led by Tomasz Kapłon at Wrocław University of Science and Technology. The focus of the repository revolves around solving the Traveling Salesman Problem (TSP) using various algorithms. The implemented algorithms include Tabu Search, Held-Karp, Brute Force, and Ant Colony Optimization. The code is written in C++.

## Project Structure

The project is organized into the following directories, each containing the implementation and necessary files for a specific algorithm:

- **Ant Colony**
- **Brute Force**
- **Held-Karp**
- **Tabu Search**

### Ant Colony
Ant Colony Optimization is inspired by the foraging behavior of ants. It uses artificial ants to traverse the solution space, laying pheromones on paths. The probability of choosing a path is influenced by the amount of pheromone, leading to emergent optimal paths.

#### Files:
- `main.cpp`: Contains the implementation of the Ant Colony Optimization algorithm.
- `conf.ini`: Configuration file specifying the parameters for the algorithm.
- `.txt files`: Various datasets for testing the algorithm.
- `Sprawozdanie.pdf`: Report detailing the implementation and results.

### Brute Force
Brute Force is a straightforward approach that exhaustively explores all possible solutions to find the optimal one. While not efficient for large problem instances, it guarantees an optimal solution.

#### Files:
- `algorytm.cpp`: Contains the implementation of the Brute Force algorithm.
- `algorytm.exe`: Executable file of the algorithm.
- `.txt files`: Various datasets for testing the algorithm.
- `Sprawozdanie.pdf`: Report detailing the implementation and results.

### Held-Karp
Held-Karp is a dynamic programming algorithm specifically designed for solving the Traveling Salesman Problem. It optimally computes the shortest tour through all cities by considering subproblems and building up the solution.

#### Files:
- `main.cpp`: Contains the implementation of the Held-Karp algorithm.
- `plik.ini`: Configuration file specifying the parameters for the algorithm.
- `.txt files`: Various datasets for testing the algorithm.
- `Sprawozdanie.pdf`: Report detailing the implementation and results.

### Tabu Search
Tabu Search is a local search algorithm that explores the solution space iteratively, aiming to find an optimal solution. It utilizes a memory structure to avoid revisiting previously explored solutions, preventing cycling.

#### Files:
- `main1.cpp` & `main2.cpp`: Contains the implementation of two variations of the Tabu Search algorithm.
- `atest.ini`: Configuration file specifying the parameters for the algorithm.
- `.txt files`: Various datasets for testing the algorithm.
- `Sprawozdanie.pdf`: Report detailing the implementation and results.

## Detailed Descriptions of Algorithms

### Ant Colony Optimization
Ant Colony Optimization (ACO) simulates the behavior of ants searching for food. The algorithm maintains a population of solutions, which are iteratively improved by simulated ants laying down pheromones. Pheromone levels influence the probability of path selection, promoting paths that lead to better solutions over time.

**Key Concepts:**
- **Pheromones**: Chemical trails laid by ants that influence the paths taken by subsequent ants.
- **Heuristic Information**: Information about the problem that helps in making more informed decisions.
- **Exploration and Exploitation**: Balancing between exploring new paths and exploiting known good paths.

### Brute Force
The Brute Force approach systematically enumerates all possible tours and selects the one with the minimum cost. It is the simplest method to solve TSP but is computationally infeasible for large instances due to its exponential time complexity.

**Key Concepts:**
- **Exhaustive Search**: Evaluating all possible solutions to find the best one.
- **Optimal Solution**: Guarantees finding the best possible solution.

### Held-Karp
The Held-Karp algorithm is a dynamic programming approach that solves TSP in \(O(n^2 2^n)\) time, which is significantly faster than the brute force approach for moderately sized instances. It breaks down the problem into smaller subproblems, solving each just once and storing the results.

**Key Concepts:**
- **Dynamic Programming**: Solving complex problems by breaking them down into simpler subproblems.
- **Subproblem Overlap**: Reusing solutions to subproblems to construct solutions to larger problems.

### Tabu Search
Tabu Search is a metaheuristic that guides a local search procedure to explore the solution space beyond local optimality. It uses memory structures that describe the visited solutions (or user-defined sets of moves) to prevent cycling.

**Key Concepts:**
- **Tabu List**: A memory structure that keeps track of recent moves to avoid cycling.
- **Aspiration Criteria**: Conditions under which a move can be accepted even if it is tabu.

## Usage

To compile and run the algorithms, navigate to the corresponding directory and use a C++ compiler. For example, to compile and run the Ant Colony Optimization algorithm:

```bash
cd Ant Colony
g++ main.cpp -o ant_colony
./ant_colony
```

Ensure that you have the required datasets and configuration files in the same directory as the executable.
