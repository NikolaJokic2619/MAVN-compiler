#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(TokenList &tokenList)
    : tokenList(tokenList),
      errorFound(false),
      instructionPosition(0),
      functionName(""),
      nextLabel("")
{
    currentToken = tokenList.begin();
}

SyntaxAnalysis::~SyntaxAnalysis()
{
    for (Variable *v : variables)
        delete v;

    for (Instruction *i : instructions)
        delete i;
}

bool SyntaxAnalysis::Do()
{
    Q();

    if (!errorFound)
    {
        cout << "Syntax OK" << endl;
    }

    return !errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token token)
{
    cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

TokenType SyntaxAnalysis::currentType()
{
    if (currentToken == tokenList.end())
        return T_END_OF_FILE;

    return currentToken->getType();
}

std::string SyntaxAnalysis::currentValue()
{
    if (currentToken == tokenList.end())
        return "";

    return currentToken->getValue();
}

void SyntaxAnalysis::eat(TokenType t)
{
    if (errorFound)
        return;
    else
    {
        if (currentType() == t)
        {
            ++currentToken;

            while (currentToken != tokenList.end() && currentToken->getType() == T_COMMENT)
            {
                ++currentToken;
            }
        }
        else
        {
            errorFound = true;
            errorToken = *currentToken;
            printSyntaxError(errorToken);
        }
    }
}

Variable *SyntaxAnalysis::findVariable(const std::string &name)
{
    for (Variable *v : variables)
    {
        if (v->getName() == name)
            return v;
    }

    // variable not found
    cout << "Error: variable [" << name << "] not declared!" << endl;
    errorFound = true;
    return nullptr;
}

/*
Q()  →  S ; L
S()  →  _mem | _reg | _func | id: | E
L()  →  eof ili jos Q
E()  →  add | addi | sub | la | lw | li | sw | b | bltz | nop
*/

void SyntaxAnalysis::Q()
{
    if (errorFound == false)
    {
        S();
        eat(T_SEMI_COL);
        L();
    }
}

void SyntaxAnalysis::S()
{
    if (errorFound)
        return;

    if (currentType() == T_MEM)
    {
        eat(T_MEM);

        string varName = currentValue();
        eat(T_M_ID);

        int varValue = stoi(currentValue());
        eat(T_NUM);

        Variable *var = new Variable(varName, instructionPosition++, Variable::MEM_VAR, varValue);
        variables.push_back(var);
    }
    else if (currentType() == T_REG)
    {
        eat(T_REG);

        string varName = currentValue();
        eat(T_R_ID);

        Variable *var = new Variable(varName, instructionPosition++, Variable::MEM_VAR);
        variables.push_back(var);
    }
    else if (currentType() == T_FUNC)
    {
        eat(T_FUNC);

        functionName = currentValue();
        eat(T_ID);
    }
    else if (currentType() == T_ID)
    {
        string label = currentValue();
        eat(T_ID);
        eat(T_COL);

        nextLabel = label;
        E();
    }
    else
        E();
}

void SyntaxAnalysis::L()
{
    if (errorFound == false)
    {
        if (currentType() == T_END_OF_FILE)
            eat(T_END_OF_FILE);
        else
            Q();
    }
}

void SyntaxAnalysis::E()
{
    if (errorFound)
        return;

    Instruction *instr = new Instruction();
    instr->setPosition(instructionPosition++);

    if (nextLabel.empty())
    {
        instr->setLabelName(nextLabel);
        nextLabel = "";
    }

    switch (currentType())
    {
    case T_ADD:
    {
        eat(T_ADD);
        instr->setType(I_ADD);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string src1Name = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string src2Name = currentValue();
        eat(T_R_ID);

        instr->getDst().push_back(findVariable(dstName));
        instr->getSrc().push_back(findVariable(src1Name));
        instr->getSrc().push_back(findVariable(src2Name));
        break;
    }
    case T_ADDI:
    {
        eat(T_ADDI);
        instr->setType(I_ADDI);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string src1Name = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        int imm = stoi(currentValue());
        eat(T_NUM);

        instr->getDst().push_back(findVariable(dstName));
        instr->getSrc().push_back(findVariable(src1Name));
        instr->setOffset(imm);
        break;
    }
    case T_SUB:
    {
        eat(T_SUB);
        instr->setType(I_SUB);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string src1Name = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string src2Name = currentValue();
        eat(T_R_ID);

        instr->getDst().push_back(findVariable(dstName));
        instr->getSrc().push_back(findVariable(src1Name));
        instr->getSrc().push_back(findVariable(src2Name));
        break;
    }
    case T_LA:
    {
        eat(T_LA);
        instr->setType(I_LA);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string srcName = currentValue();
        eat(T_M_ID);

        instr->getDst().push_back(findVariable(dstName));
        instr->getSrc().push_back(findVariable(srcName));
        break;
    }
    case T_LW:
    {
        eat(T_LW);
        instr->setType(I_LW);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        int offset = stoi(currentValue());
        eat(T_NUM);
        eat(T_L_PARENT);
        string baseName = currentValue();
        eat(T_R_ID);
        eat(T_R_PARENT);

        instr->getDst().push_back(findVariable(dstName));
        instr->getSrc().push_back(findVariable(baseName));
        instr->setOffset(offset);
        break;
    }
    case T_LI:
    {
        eat(T_LI);
        instr->setType(I_LI);

        string dstName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        int imm = stoi(currentValue());
        eat(T_NUM);

        instr->getDst().push_back(findVariable(dstName));
        instr->setOffset(imm);
        break;
    }
    case T_SW:
    {
        eat(T_SW);
        instr->setType(I_SW);

        string srcName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        int offset = stoi(currentValue());
        eat(T_NUM);
        eat(T_L_PARENT);
        string baseName = currentValue();
        eat(T_R_ID);
        eat(T_R_PARENT);

        instr->getSrc().push_back(findVariable(srcName));
        instr->getSrc().push_back(findVariable(baseName));
        instr->setOffset(offset);
        break;
    }
    case T_B:
    {
        eat(T_B);
        instr->setType(I_B);

        string label = currentValue();
        eat(T_ID);
        instr->setBranchLabel(label);
        break;
    }
    case T_BLTZ:
    {
        eat(T_BLTZ);
        instr->setType(I_BLTZ);

        string srcName = currentValue();
        eat(T_R_ID);
        eat(T_COMMA);
        string label = currentValue();
        eat(T_ID);

        instr->getSrc().push_back(findVariable(srcName));
        instr->setBranchLabel(label);
        break;
    }
    case T_NOP:
    {
        eat(T_NOP);
        instr->setType(I_NOP);
        break;
    }
    default:
    {
        errorFound = true;
        errorToken = *currentToken;
        delete instr;
        return;
    }
    }

    instructions.push_back(instr);
}
