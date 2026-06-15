#include "CodeGeneration.h"
#include <iterator>
#include <filesystem>

using namespace std;

CodeGeneration::CodeGeneration(Variables &vars, Instructions &instrs, string functionName, string out) : variables(vars), instructions(instrs), function(functionName), output(out) {}

void CodeGeneration::Do()
{
    filesystem::create_directory("../output");
    ofstream out(output);

    if (!out.is_open())
    {
        cout << "Cannot open file" << endl;
        return;
    }

    generateData(out);
    generateText(out);

    out.close();
}

string CodeGeneration::getRegisterName(Variable *var)
{
    switch (var->getAssignment())
    {
    case t0:
        return "$t0";
    case t1:
        return "$t1";
    case t2:
        return "$t2";
    case t3:
        return "$t3";
    default:
        return "$t0";
    }
}

void CodeGeneration::generateData(ofstream &out)
{
    out << ".globl " << function << endl
        << endl;
    out << ".data" << endl;

    for (Variable *var : variables)
    {
        if (var->getType() == Variable::MEM_VAR)
            out << var->getName() << ":\t.word " << var->getValue() << endl;
    }

    out << endl;
}

void CodeGeneration::generateText(ofstream &out)
{
    out << ".text" << endl;
    out << function << ":" << endl;

    /*
        I_NO_TYPE = 0,
        I_ADD,
        I_ADDI,
        I_SUB,
        I_LA,
        I_LI,
        I_LW,
        I_SW,
        I_BLTZ,
        I_B,
        I_NOP
    */
    for (Instruction *instr : instructions)
    {
        if (instr->getLabelName() != "")
        {
            out << instr->getLabelName() << ":" << endl;
        }

        switch (instr->getType())
        {
        case I_ADD:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            out << "\tadd\t"
                << getRegisterName(dst) << ", "
                << getRegisterName(src1) << ", "
                << getRegisterName(src2) << endl;
            break;
        }
        case I_ADDI:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            out << "\taddi\t"
                << getRegisterName(dst) << ", "
                << getRegisterName(src1) << ", "
                << instr->getOffset() << endl;
            break;
        }
        case I_SUB:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            out << "\tsub\t"
                << getRegisterName(dst) << ", "
                << getRegisterName(src1) << ", "
                << getRegisterName(src2) << endl;
            break;
        }
        case I_LA:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            out << "\tla\t"
                << getRegisterName(dst) << ", "
                << src1->getName() << endl;
            break;
        }
        case I_LI:
        {
            Variable *dst = instr->getDst().front();

            out << "\tli\t"
                << getRegisterName(dst) << ", "
                << instr->getOffset() << endl;
            break;
        }
        case I_LW:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            out << "\tlw\t"
                << getRegisterName(dst) << ", "
                << instr->getOffset()
                << "(" << getRegisterName(src1) << ")" << endl;
            break;
        }
        case I_SW:
        {
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *base = *it;

            out << "\tsw\t"
                << getRegisterName(src1) << ", "
                << instr->getOffset()
                << "(" << getRegisterName(base) << ")" << endl;
            break;
        }
        case I_BLTZ:
        {
            Variable *src1 = instr->getSrc().front();

            out << "\tbltz\t"
                << getRegisterName(src1) << ", "
                << instr->getBranchLabel() << endl;
            break;
        }
        case I_B:
            out << "\tb\t"
                << instr->getBranchLabel() << endl;
            break;

        case I_NOP:
            out << "\tnop" << endl;
            break;

        case I_MUL:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            out << "\tmul\t"
                << getRegisterName(dst) << ", "
                << getRegisterName(src1) << ", "
                << getRegisterName(src2) << endl;
            break;
        }
        case I_AND:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            out << "\tand\t"
                << getRegisterName(dst) << ", "
                << getRegisterName(src1) << ", "
                << getRegisterName(src2) << endl;
            break;
        }
        case I_BEQ:
        {
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            out << "\tbeq\t"
                << getRegisterName(src1) << ", "
                << getRegisterName(src2) << ", "
                << instr->getBranchLabel() << endl;
            break;
        }
        default:
            break;
        }
    }
}
