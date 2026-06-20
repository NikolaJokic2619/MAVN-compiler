#include "ResourceAllocation.h"

using namespace std;

const int NORMAL_COLORS = __REG_NUMBER__ - 1;

ResourceAllocation::ResourceAllocation(InterferenceGraph &ig) : interferenceGraph(ig) {}

Regs ResourceAllocation::getColor(Variable *notColoredVar)
{
    Regs availableRegs[3] = {t0, t1, t2};

    bool usedRegs[NORMAL_COLORS];

    Variables &vars = interferenceGraph.getRegVariables();

    for (int r = 0; r < NORMAL_COLORS; r++)
    {
        usedRegs[r] = false;
    }

    for (Variable *var : vars)
    {
        if (var == notColoredVar)
            continue;

        // promenljive koje su vec prosute u memoriju ne zauzimaju registar
        if (var->isSpilled())
            continue;

        if (interferenceGraph.hasConnection(notColoredVar, var))
        {
            Regs assigned = var->getAssignment();

            if (assigned != no_assign)
            {
                for (int i = 0; i < NORMAL_COLORS; i++)
                {
                    if (assigned == availableRegs[i])
                    {
                        usedRegs[i] = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < NORMAL_COLORS; i++)
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

    int spillCount = 0;

    while (!simplificationStack.empty())
    {
        current = simplificationStack.top();
        simplificationStack.pop();

        Regs color;

        if (previous == NULL)
        {
            color = t0;
        }
        else
        {
            color = getColor(current);
        }

        if (color == no_assign)
        {
            // nema slobodnog registra -> SPILL u memoriju umesto da odustanemo
            current->setSpilled(true);
            current->setAssignment(no_assign);
            current->setSpillLabel("spill_" + current->getName());
            spillCount++;

            cout << "Variable [" << current->getName() << "] spilled to memory ("
                 << current->getSpillLabel() << ")" << endl;
        }
        else
        {
            current->setAssignment(color);
        }

        previous = current;
    }

    if (spillCount > 0)
        cout << "\nResource allocation completed WITH " << spillCount << " spill(s)." << endl;
    else
        cout << "\nResource allocation completed successfully, no spills needed." << endl;
}

void ResourceAllocation::printAllocation()
{
    cout << "\n*****Resource Allocation*****" << endl;

    for (Variable *v : interferenceGraph.getRegVariables())
    {
        cout << v->getName() << ": ";

        if (v->isSpilled())
        {
            cout << "SPILLED -> " << v->getSpillLabel();
        }
        else
        {
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
        }

        cout << endl;
    }
}