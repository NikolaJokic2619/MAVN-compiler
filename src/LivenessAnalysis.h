#pragma once

#include "IR.h"
#include <iostream>
#include <string>

class LivenessAnalysis
{
public:
    LivenessAnalysis(Instructions &instructions);

    void Do();
    void printSets();
    std::string varToStr(Variables &vars);

private:
    Instructions &instructions_liveness;

    void findUseDef();
    void findInOut();

    bool repeat;

    bool alreadyInSet(Variables &set, Variable *var);

    void addToSet(Variables &dest, Variables &source);
};
