#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "literal.h"
#include "clause.h"

class Assignment {

    private:
        int value;
        Clause antecedent;
        int decision_level;

    public:

    Assignment(){
        value = 0;
        antecedent = Clause();
        decision_level = 0;
    }

    Assignment(int value, Clause antecedent, int decision_level){
        this->value = value;
        this->antecedent = antecedent;
        this->decision_level = decision_level;
    }

    // Get the antecedent of the assignment
    Clause getAntecedent() const{
        return antecedent;
    }

    // Get the decision level of the assignment
    int getDecisionLevel() const{
        return decision_level;
    }

    // Get the value of the assignment
    int getValue() const{
        return value;
    }

};

#endif