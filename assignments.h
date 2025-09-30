#ifndef ASSIGNMENTS_H
#define ASSIGNMENTS_H

#include <iostream>
#include <unordered_map>
#include <vector> // Include vector
#include "literal.h"
#include "assignment.h"
#include "clause.h"
#include "formula.h"


class Assignments : public std::unordered_map<int, Assignment> {

private:
    int decision_level;
    std::vector<int> trail; // The assignment trail

public:
    Assignments() : std::unordered_map<int, Assignment>(), decision_level(0) {}

    // Get the decision level
    bool value(Literal literal) {
        if (find(literal.getVariable()) == end()) {
            return false; // Should not be called on unassigned literals in a valid flow
        }
        if (literal.isNegated()) {
            return !at(literal.getVariable()).getValue();
        } else {
            return at(literal.getVariable()).getValue();
        }
    }

    // Assign a value to a variable
    void assign(int variable, bool value, Clause antecedent) {
        this->operator[](variable) = Assignment(value, antecedent, decision_level);
        trail.push_back(variable); // Add the assignment to our trail
    }

    // Unassign a variable
    void unassign(int variable) {
        erase(variable);
    }

    // Increase the decision level
    void increaseDecisionLevel() {
        decision_level++;
    }

    int getDecisionLevel() const { // Added const for correctness
        return decision_level;
    }

    void setDecisionLevel(int level) {
        decision_level = level;
    }
    
    // Getter for the trail
    const std::vector<int>& getTrail() const {
        return trail;
    }
    
    // Method to pop from the trail during backtrack
    void popTrail() {
        if (!trail.empty()) {
            trail.pop_back();
        }
    }

    // Check whether the assignments actually satisfies the formula
    bool satisfy(Formula formula) {
        for (Clause clause : formula) {
            bool clause_satisfied = false;
            for (Literal literal : clause) {
                if (find(literal.getVariable()) != end() && value(literal)) {
                    clause_satisfied = true;
                    break;
                }
            }
            if (!clause_satisfied) {
                return false;
            }
        }
        return true;
    }
};

#endif