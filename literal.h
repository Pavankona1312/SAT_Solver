#ifndef LITERAL_H
#define LITERAL_H

#include <iostream>
#include <string>

class Literal{

    private:
        int variable;
        bool negated;

    public:

    Literal(){
        variable = 0;
        negated = false;
    }

    Literal(int variable, bool negated){
        this->variable = variable;
        this->negated = negated;
    }

    // Negate the literal
    Literal negate(){
        return Literal(variable, !negated);
    }

    // Get the variable of the literal
    int getVariable() const{
        return variable;
    }

    // Check if the literal is negated
    bool isNegated() const{
        return negated;
    }

    std::string to_string() const{
        std::string str = "";
        if(negated){
            str += "¬";
        }
        str += std::to_string(variable);
        return str;
    }

    std::ostream& operator<<(std::ostream& os){
        os << to_string();
        return os;
    }

    bool operator==(const Literal& other) const{
        return variable == other.variable && negated == other.negated;
    }

    bool operator<(const Literal& other) const{
        return variable < other.variable || (variable == other.variable && negated < other.negated);
    }
    
};

#endif