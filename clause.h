#ifndef CLAUSE_H
#define CLAUSE_H

#include <iostream>
#include <vector>
#include "literal.h"

class Clause {

    std::vector<Literal> literals;

    public:

    Clause(){
        literals = std::vector<Literal>();
    }

    Clause(std::vector<Literal> literals){
        this->literals = literals;
    }

    std::vector<Literal>::iterator begin() { return literals.begin(); }
    std::vector<Literal>::iterator end() { return literals.end(); }

    std::string to_string() const{
        std::string str = "";
        for(int i = 0; i < literals.size(); i++){
            str += literals[i].to_string();
            if(i < literals.size() - 1){
                str += " ∨ ";
            }
        }
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const Clause& clause){
        os << clause.to_string();
        return os;
    }

    void addLiteral(Literal literal){
        literals.push_back(literal);
    }

    // Get the length of the clause
    int length() const{
        return literals.size();
    }

    bool operator==(const Clause& other) const{
        if(literals.size() != other.literals.size()){
            return false;
        }
        for(int i = 0; i < literals.size(); i++){
            if (!(literals[i] == other.literals[i])) {
                return false;
            }
        }
        return true;
    }
};

#endif