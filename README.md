

# C++ CDCL SAT Solver: Running Instructions

This guide provides the necessary steps to compile and run the SAT solver.


## Compilation

To compile the project, navigate to the source directory and use a C++ compiler like `g++`. This will create an executable file named `solver`.

```sh
g++ -std=c++17 -o solver main.cpp solver.cpp
```


## Execution

Run the compiled program from your terminal, providing the path to a `.cnf` file as the only argument.

### Syntax

```sh
./solver <path_to_your_file.cnf>
```

### Example

If you have a file named `example.cnf` in the same directory:

```sh
./solver example.cnf
```


## Input Format

The solver accepts input in the standard **DIMACS CNF** format.
  * **Problem Line:** The file should contain one problem line: `p cnf <num_variables> <num_clauses>`.
  * **Clause Lines:** Each subsequent line represents a clause.
      * A positive integer `x` represents the variable $x$.
      * A negative integer `-x` represents the negated variable $\neg x$.
      * Each clause line ends with a `0`.

### Example Input (`example.cnf`)

```
p cnf 3 4
1 -2 0
1 -3 0
-2 -3 0
2 3 0
```

This file describes the formula: $(x_1 \lor \neg x_2) \land (x_1 \lor \neg x_3) \land (\neg x_2 \lor \neg x_3) \land (x_2 \lor x_3)$


## Output

The program will print the result to the console.

  * If the formula is **satisfiable**, the output will be `SAT` followed by a satisfying assignment:

    ```
    SAT
    1 -2 3 0
    ```

  * If the formula is **unsatisfiable**, the output will be `UNSAT`:

    ```
    UNSAT
    ```