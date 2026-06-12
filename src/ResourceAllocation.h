#pragma once

#include "InterferenceGraph.h"
#include "Types.h"

#include <stack>

class ResourceAllocation
{
public:
    ResourceAllocation(InterferenceGraph &ig);

    void Do();
    void printAllocation();

private:
    Regs getColor(Variable *var);

    InterferenceGraph &interferenceGraph;

    std::stack<Variable *> simplificationStack;
};
