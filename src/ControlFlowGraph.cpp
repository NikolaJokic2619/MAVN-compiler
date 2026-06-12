#include <iostream>
#include <iomanip>

#include "ControlFlowGraph.h"

using namespace std;

ControlFlowGraph::ControlFlowGraph(Instructions &instructions) : instructions_cfg(instructions) {}

void ControlFlowGraph::Do()
{
    if (!instructions_cfg.empty())
    {
        auto it = instructions_cfg.begin();
        auto next = it;
        ++next;

        while (next != instructions_cfg.end())
        {
            (*it)->getSucc().push_back(*next);
            (*next)->getPred().push_back(*it);

            ++it;
            ++next;
        }
    }
}

void ControlFlowGraph::printGraph()
{
    cout << "\n*****Control Flow Graph*****" << endl;

    for (Instruction *instr : instructions_cfg)
    {
        cout << "Instr " << instr->getPosition() << " -> succ [";
        for (Instruction *succ : instr->getSucc())
        {
            cout << succ->getPosition();
        }
        cout << "]" << endl;
    }
}