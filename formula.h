#ifndef FORMULA_H
#define FORMULA_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "clause.h"
#include "literal.h"

class Formula {

    private:
        std::vector<Clause> clauses;
        std::set<int> variables;

    public:

    Formula(){
        clauses = std::vector<Clause>();
        variables = std::set<int>();
    }

    Formula(std::vector<Clause> clauses){
        // this->clauses = clauses;
        for(Clause clause : clauses){
            std::set<Literal> clause_set(clause.begin(), clause.end());
            this->clauses.push_back(Clause(std::vector<Literal>(clause_set.begin(), clause_set.end())));
            for(Literal literal : clause){
                variables.insert(literal.getVariable());
            }
        }
    }

    std::vector<Clause>::iterator begin() { return clauses.begin(); }
    std::vector<Clause>::iterator end() { return clauses.end(); }

    std::string to_string() const{
        std::string str = "";
        for(int i = 0; i < clauses.size(); i++){
            str += clauses[i].to_string();
            if(i < clauses.size() - 1){
                str += " ∧ ";
            }
        }
        return str;
    }

    std::ostream& operator<<(std::ostream& os){
        os << to_string();
        return os;
    }

    void addClause(Clause clause){
        // clauses.push_back(clause);
        for (Clause c : clauses) {
            std::cout << c.to_string() << std::endl;
        }
        if (find(clauses.begin(), clauses.end(), clause) == clauses.end()) {
            clauses.push_back(clause);
            for(Literal literal : clause){
                variables.insert(literal.getVariable());
            }
        }
    }

    // Get the variables in the formula
    std::set<int> getVariables() const{
        return variables;
    }

    // Get the clauses in the formula
    std::vector<Clause> getClauses() const{
        return clauses;
    }

    // Get the length of the formula
    int length() const{
        return clauses.size();
    }

};

#endif