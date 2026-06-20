#pragma once

#include "IR.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class CodeGeneration
{
public:
    CodeGeneration(Variables &vars, Instructions &instrs, string functionName, string out);

    void Do();

private:
    string getRegisterName(Variable *var);
    void generateData(ofstream &out);
    void generateText(ofstream &out);

    // spilling - vraca ime registra za operand (normalan registar ili $t3 ako je prosut)
    string getOperand(Variable *var);
    // ako je promenljiva prosuta, ucitaj je u $t3 PRE upotrebe (za src operande)
    void emitSpillLoad(ofstream &out, Variable *var);
    // ako je promenljiva prosuta, sacuvaj $t3 nazad u memoriju POSLE upotrebe (za dst operande)
    void emitSpillStore(ofstream &out, Variable *var);

    Variables &variables;
    Instructions &instructions;
    string function;
    string output;
};