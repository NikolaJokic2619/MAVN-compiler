#pragma once

#include "IR.h"

class ControlFlowGraph
{
public:
    ControlFlowGraph(Instructions &instructions);

    void Do();
    void printGraph();

private:
    Instructions &instructions_cfg;
};