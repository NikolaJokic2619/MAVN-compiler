#include "InterferenceGraph.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables &vars, Instructions &instrs) : variables(vars), instructions(instrs) {}

void InterferenceGraph::Do()
{
    for (Variable *v : variables)
    {
        if (v->getType() == Variable::REG_VAR)
            regVariables.push_back(v);
    }

    int n = regVariables.size();

    matrix.resize(n, vector<int>(n, 0));

    for (Instruction *instr : instructions)
    {
        processSet(instr->getIn());
        processSet(instr->getOut());
    }
}

void InterferenceGraph::printGraph()
{
    cout << "\n*****Interference Graph*****" << endl;

    cout << setw(6) << " ";
    for (Variable *v : regVariables)
        cout << setw(6) << v->getName();
    cout << endl;

    for (int i = 0; i < regVariables.size(); i++)
    {
        auto it = regVariables.begin();
        advance(it, i);
        cout << setw(6) << (*it)->getName();

        for (int j = 0; j < regVariables.size(); j++)
            cout << setw(6) << matrix[i][j];

        cout << endl;
    }
}

int InterferenceGraph::getIndex(Variable *v)
{
    int ind = 0;

    for (Variable *reg : regVariables)
    {
        if (v == reg)
            return ind;
        ind++;
    }

    return -1;
}

void InterferenceGraph::addConnection(Variable *v1, Variable *v2)
{
    int ind1 = getIndex(v1);
    int ind2 = getIndex(v2);

    if (ind1 == -1 || ind2 == -1)
    {
        return;
    }

    matrix[ind1][ind2] = 1;
    matrix[ind2][ind1] = 1;
}

bool InterferenceGraph::hasConnection(Variable *v1, Variable *v2)
{
    int ind1 = getIndex(v1);
    int ind2 = getIndex(v2);

    if (ind1 == -1 || ind2 == -1)
    {
        return false;
    }

    return matrix[ind1][ind2] == 1;
}

void InterferenceGraph::processSet(Variables &set)
{
    for (Variable *v1 : set)
    {
        for (Variable *v2 : set)
        {
            if (v1->getType() != Variable::REG_VAR || v2->getType() != Variable::REG_VAR || v1 == v2)
                continue;

            addConnection(v1, v2);
        }
    }
}