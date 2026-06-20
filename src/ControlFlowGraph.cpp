#include <iostream>
#include <iomanip>
#include <map>

#include "ControlFlowGraph.h"

using namespace std;

ControlFlowGraph::ControlFlowGraph(Instructions &instructions) : instructions_cfg(instructions) {}

void ControlFlowGraph::Do()
{
    if (instructions_cfg.empty())
        return;

    map<string, Instruction *> labelMap;

    for (Instruction *instr : instructions_cfg)
    {
        if (instr->getLabelName() != "")
        {
            labelMap[instr->getLabelName()] = instr;
        }
    }

    auto it = instructions_cfg.begin();
    auto next = it;
    ++next;

    while (next != instructions_cfg.end())
    {
        Instruction *current = *it;

        if (next != instructions_cfg.end() && current->getType() != I_B)
        {
            current->getSucc().push_back(*next);
            (*next)->getPred().push_back(current);
        }

        if (current->getType() == I_B || current->getType() == I_BLTZ || current->getType() == I_BEQ)
        {
            string target = current->getBranchLabel();
            if (labelMap.find(target) != labelMap.end())
            {
                Instruction *targetInstr = labelMap[target];
                current->getSucc().push_back(targetInstr);
                targetInstr->getPred().push_back(current);
            }
        }

        ++it;
        if (next != instructions_cfg.end())
            ++next;
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
