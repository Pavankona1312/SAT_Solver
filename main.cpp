#include "formula.h"
#include "clause.h"
#include "assignments.h"
#include "solver.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

Formula parseFormula(std::string formula_str){
    Formula formula;
    std::vector<Clause> clauses;
    std::string clause_str = "";
    
    std::istringstream iss(formula_str);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string token;

        if (!line.empty() && line[0] != 'p' && line[0] != 'c') {
            Clause clause = Clause();
            while (lineStream >> token) {
                // std::cout << token << std::endl;
                int literal = std::stoi(token);

                if (literal == 0) {
                    formula.addClause(clause);
                    clause = Clause();
                } else {
                    int variable = abs(literal);
                    bool negated = literal < 0;
                    clause.addLiteral(Literal(variable, negated));
                }
            }
        }
    }

    return formula;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <cnf_file>" << std::endl;
        return 1;
    }

    std::string cnf_file = argv[1];
    std::ifstream file(cnf_file);

    if (!file.is_open()) {
        std::cout << "Error: could not open file " << cnf_file << std::endl;
        return 1;
    }

    std::string dimacs_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    Formula formula = parseFormula(dimacs_string);
    // for (Clause clause : formula) {
    //     for (Literal literal : clause) {
    //         std::cout << literal.getVariable() * (literal.isNegated() ? -1 : 1) << " ";
    //     }
    //     std::cout << "0" << std::endl;
    // }
    Assignments result;
    Solver solver;
    result = solver.CDCLSolve(formula);



    if (result.size() == 0) {
        std::cout << "UNSAT" << std::endl;
    } else {
        std::cout << "SAT" << std::endl;
        for (auto const& [variable, assignment] : result) {
            if (assignment.getValue()) {
                std::cout << variable << " ";
            } else {
                std::cout << -variable << " ";
            }
        }
        std::cout << "0" << std::endl;
    }
}