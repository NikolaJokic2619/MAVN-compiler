#pragma once

#include "IR.h"
#include <iostream>
#include <string>
#include <fstream>

class CodeGeneration
{
public:
    CodeGeneration(Variables &vars, Instructions &instrs, std::string functionName, std::string out);

    void Do();

private:
    Variables &variables;
    Instructions &instructions;
    std::string function;
    std::string output;

    std::string getRegisterName(Variable *var);
    void generateData(std::ofstream &out);
    void generateText(std::ofstream &out);
};