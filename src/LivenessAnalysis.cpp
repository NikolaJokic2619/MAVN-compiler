#include <iomanip>

#include "LivenessAnalysis.h"

using namespace std;

LivenessAnalysis::LivenessAnalysis(Instructions &instructions) : instructions_liveness(instructions) {}

void LivenessAnalysis::Do()
{
    findUseDef();
    findInOut();
}

void LivenessAnalysis::printSets()
{
    cout << "\n*****Liveness Analysis Sets*****" << endl;

    const int pos_w = 8;
    const int var_w = 10;

    cout << left
         << setw(pos_w) << "POS"
         << setw(var_w) << "USE"
         << setw(var_w) << "DEF"
         << setw(var_w) << "IN"
         << setw(var_w) << "OUT" << "\n";

    cout << string(44, '-') << "\n";

    for (Instruction *instr : instructions_liveness)
    {
        cout << left
             << setw(pos_w) << instr->getPosition()
             << setw(var_w) << varToStr(instr->getUse())
             << setw(var_w) << varToStr(instr->getDef())
             << setw(var_w) << varToStr(instr->getIn())
             << setw(var_w) << varToStr(instr->getOut()) << "\n";
    }
}

string LivenessAnalysis::varToStr(Variables &vars)
{
    string str;

    for (Variable *var : vars)
    {
        str += var->getName() + " ";
    }

    return str;
}

void LivenessAnalysis::findUseDef()
{
    for (Instruction *instr : instructions_liveness)
    {
        for (Variable *v : instr->getDst())
        {
            if (!alreadyInSet(instr->getDef(), v))
            {
                instr->getDef().push_back(v);
            }
        }

        for (Variable *v : instr->getSrc())
        {
            if (!alreadyInSet(instr->getUse(), v))
            {
                instr->getUse().push_back(v);
            }
        }
    }
}

void LivenessAnalysis::findInOut()
{
    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto it = instructions_liveness.rbegin(); it != instructions_liveness.rend(); ++it)
        {
            Instruction *instr = *it;

            Variables oldIn = instr->getIn();
            Variables oldOut = instr->getOut();

            for (Instruction *succ : instr->getSucc())
                addToSet(instr->getOut(), succ->getIn());

            addToSet(instr->getIn(), instr->getUse());

            for (Variable *v : instr->getOut())
            {
                if (!alreadyInSet(instr->getDef(), v))
                {
                    if (!alreadyInSet(instr->getIn(), v))
                    {
                        instr->getIn().push_back(v);
                    }
                }
            }

            if (oldIn != instr->getIn() || oldOut != instr->getOut())
                changed = true;
        }
    }
}

bool LivenessAnalysis::alreadyInSet(Variables &set, Variable *var)
{
    for (Variable *v : set)
    {
        if (v == var)
            return true;
    }
    return false;
}

void LivenessAnalysis::addToSet(Variables &dst, Variables &src)
{
    for (Variable *v : src)
    {
        if (!alreadyInSet(dst, v))
            dst.push_back(v);
    }
}
