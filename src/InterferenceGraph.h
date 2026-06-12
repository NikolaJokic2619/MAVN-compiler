#pragma once

#include "IR.h"
#include <iostream>
#include <iomanip>
#include <vector>

class InterferenceGraph
{
public:
    InterferenceGraph(Variables &vars, Instructions &instrs);

    void Do();
    void printGraph();

    bool hasConnection(Variable *v1, Variable *v2);

    std::vector<std::vector<int>> &getMatrix() { return matrix; }

    Variables &getRegVariables() { return regVariables; }

private:
    void addConnection(Variable *v1, Variable *v2);

    void processSet(Variables &set);

    Variables &variables;
    Instructions &instructions;

    Variables regVariables;
    std::vector<std::vector<int>> matrix;

    int getIndex(Variable *v);
};
