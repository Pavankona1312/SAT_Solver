#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include "assignments.h"
#include "clause.h"

// ... (enums remain the same) ...

enum ClauseStatus {
    SATISFIED,
    UNIT,
    UNSATISFIED,
    UNRESOLVED
};

enum UnitPropagationStatus {
    UNIT_CONFLICT,
    UNIT_UNRESOLVED
};

class Solver {
public:
    Solver() {};

    // ... (checkClauseStatus, unitPropagate, addClause, allAssigned, pickVariable are unchanged from the previous fix) ...
    ClauseStatus checkClauseStatus(Clause& clause, Assignments& assignments, Literal& unit_literal) {
        int unassigned_count = 0;
        int false_count = 0;
        
        for (Literal literal : clause) {
            if (assignments.find(literal.getVariable()) == assignments.end()) {
                unassigned_count++;
                unit_literal = literal;
            } else {
                bool value = assignments.value(literal);
                if (value) {
                    return SATISFIED;
                } else {
                    false_count++;
                }
            }
        }

        if (false_count == clause.length()) {
            return UNSATISFIED;
        }
        if (unassigned_count == 1 && false_count == clause.length() - 1) {
            return UNIT;
        }
        
        return UNRESOLVED;
    }

    std::pair<UnitPropagationStatus, Clause> unitPropagate(Assignments& assignments, Formula& formula) {
        bool changed = true;
        while (changed) {
            changed = false;
            for (Clause& clause : formula) {
                Literal unit_literal;
                ClauseStatus status = checkClauseStatus(clause, assignments, unit_literal);

                if (status == UNIT) {
                    if (assignments.find(unit_literal.getVariable()) == assignments.end()) {
                        assignments.assign(unit_literal.getVariable(), !unit_literal.isNegated(), clause);
                        changed = true;
                    }
                } else if (status == UNSATISFIED) {
                    return {UNIT_CONFLICT, clause};
                }
            }
        }
        return {UNIT_UNRESOLVED, Clause()};
    }

    void addClause(Formula& formula, Clause clause) {
        formula.addClause(clause);
    }

    bool allAssigned(Formula& formula, Assignments& assignments) {
        return formula.getVariables().size() == assignments.size();
    }

    std::pair<int, bool> pickVariable(Formula& formula, Assignments& assignments) {
        for (int variable : formula.getVariables()) {
            if (assignments.find(variable) == assignments.end()) {
                return {variable, true};
            }
        }
        return {-1, false};
    }

    // CORRECTED: Backtrack now efficiently uses the trail.
    void backtrack(Assignments& assignments, int level) {
        const auto& trail = assignments.getTrail();
        while (!trail.empty()) {
            int var_to_unassign = trail.back();
            if (assignments.at(var_to_unassign).getDecisionLevel() <= level) {
                break; // Stop when we reach the target level
            }
            assignments.unassign(var_to_unassign);
            assignments.popTrail();
        }
        assignments.setDecisionLevel(level);
    }

    Clause resolve(Clause clause1, Clause clause2, int variable) {
        std::set<Literal> result_literals;
        for (Literal lit : clause1) {
            if (lit.getVariable() != variable) {
                result_literals.insert(lit);
            }
        }
        for (Literal lit : clause2) {
            if (lit.getVariable() != variable) {
                result_literals.insert(lit);
            }
        }
        return Clause(std::vector<Literal>(result_literals.begin(), result_literals.end()));
    }

    // CORRECTED: This now uses the trail for proper reverse iteration.
    std::pair<int, Clause> conflict_analysis(Clause conflicting_clause, Assignments& assignments) {
        if (assignments.getDecisionLevel() == 0) {
            return {-1, Clause()};
        }

        Clause current_clause = conflicting_clause;
        std::set<int> seen_variables;
        for (Literal lit : current_clause) {
            seen_variables.insert(lit.getVariable());
        }

        // Keep resolving until we have only one literal from the current decision level (1UIP).
        while (true) {
            int literals_at_current_level = 0;
            for (Literal lit : current_clause) {
                if (assignments.at(lit.getVariable()).getDecisionLevel() == assignments.getDecisionLevel()) {
                    literals_at_current_level++;
                }
            }
            if (literals_at_current_level <= 1) {
                break;
            }

            // Find the last assigned literal on the trail that is in our conflict clause.
            int var_to_resolve = -1;
            const auto& trail = assignments.getTrail();
            for (auto it = trail.rbegin(); it != trail.rend(); ++it) {
                int var = *it;
                if (seen_variables.count(var) && assignments.at(var).getAntecedent().length() > 0) {
                    var_to_resolve = var;
                    break;
                }
            }

            current_clause = resolve(current_clause, assignments.at(var_to_resolve).getAntecedent(), var_to_resolve);
            
            // Update seen variables for the next iteration
            seen_variables.clear();
            for (Literal lit : current_clause) {
                seen_variables.insert(lit.getVariable());
            }
        }

        // Determine backtrack level
        std::set<int> levels;
        for (Literal lit : current_clause) {
            if (assignments.find(lit.getVariable()) != assignments.end()) {
                if(assignments.at(lit.getVariable()).getDecisionLevel() < assignments.getDecisionLevel())
                    levels.insert(assignments.at(lit.getVariable()).getDecisionLevel());
            }
        }
        
        int backtrack_level = 0;
        if (!levels.empty()) {
            backtrack_level = *levels.rbegin(); // The highest level among the remaining literals
        }

        return {backtrack_level, current_clause};
    }

    // ... (CDCLSolve main loop remains the same as the previous fix) ...
    Assignments CDCLSolve(Formula formula) {
        Assignments assignments;
        
        std::pair<UnitPropagationStatus, Clause> up_result = unitPropagate(assignments, formula);
        if (up_result.first == UNIT_CONFLICT) {
            return Assignments();
        }

        while (!allAssigned(formula, assignments)) {
            assignments.increaseDecisionLevel();
            std::pair<int, bool> decision = pickVariable(formula, assignments);
            assignments.assign(decision.first, decision.second, Clause());

            while (true) {
                up_result = unitPropagate(assignments, formula);
                if (up_result.first != UNIT_CONFLICT) {
                    break;
                }
                
                Clause conflicting_clause = up_result.second;
                if (assignments.getDecisionLevel() == 0) {
                    return Assignments();
                }

                std::pair<int, Clause> analysis_result = conflict_analysis(conflicting_clause, assignments);
                int backtrack_level = analysis_result.first;
                Clause learned_clause = analysis_result.second;

                backtrack(assignments, backtrack_level);
                addClause(formula, learned_clause);
                
                // Propagate the learned clause
                Literal unit_literal;
                checkClauseStatus(learned_clause, assignments, unit_literal);
                assignments.assign(unit_literal.getVariable(), !unit_literal.isNegated(), learned_clause);
            }
        }

        return assignments;
    }
};

#endif