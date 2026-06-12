#include "ResourceAllocation.h"

using namespace std;

ResourceAllocation::ResourceAllocation(InterferenceGraph &ig) : interferenceGraph(ig) {}

Regs ResourceAllocation::getColor(Variable *notColoredVar)
{
    Regs availableRegs[4] = {t0, t1, t2, t3};

    bool usedRegs[__REG_NUMBER__];

    Variables &vars = interferenceGraph.getRegVariables();

    for (int r = 0; r < __REG_NUMBER__; r++)
    {
        usedRegs[r] = false;
    }

    for (Variable *var : vars)
    {
        if (var == notColoredVar)
            continue;

        if (interferenceGraph.hasConnection(notColoredVar, var))
        {
            Regs assigned = var->getAssignment();

            if (assigned != no_assign)
            {
                for (int i = 0; i < __REG_NUMBER__; i++)
                {
                    if (assigned == availableRegs[i])
                    {
                        usedRegs[i] = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < __REG_NUMBER__; i++)
    {
        if (usedRegs[i] == false)
        {
            return availableRegs[i];
        }
    }

    return no_assign;
}

void ResourceAllocation::Do()
{
    Variables &vars = interferenceGraph.getRegVariables();

    for (Variable *v : vars)
    {
        simplificationStack.push(v);
    }

    Variable *current, *previous;
    previous = NULL;

    while (!simplificationStack.empty())
    {
        current = simplificationStack.top();
        simplificationStack.pop();

        if (previous == NULL)
        {
            current->setAssignment((Regs)t0);
        }
        else
        {
            Regs color = getColor(current);
            if (color == no_assign)
            {
                cout << "Resource allocation failed!" << endl;
                return;
            }
            else
            {
                current->setAssignment(color);
            }
        }

        previous = current;
    }
}

void ResourceAllocation::printAllocation()
{
    cout << "\n*****Resource Allocation*****" << endl;

    for (Variable *v : interferenceGraph.getRegVariables())
    {
        cout << v->getName() << ": ";

        switch (v->getAssignment())
        {
        case t0:
            cout << "t0";
            break;
        case t1:
            cout << "t1";
            break;
        case t2:
            cout << "t2";
            break;
        case t3:
            cout << "t3";
            break;
        default:
            cout << "ERROR: UNASSIGNED REG!";
            break;
        }

        cout << endl;
    }
}