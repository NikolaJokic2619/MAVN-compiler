#include "CodeGeneration.h"
#include <iterator>

using namespace std;

CodeGeneration::CodeGeneration(Variables &vars, Instructions &instrs, string functionName, string out)
    : variables(vars), instructions(instrs), function(functionName), output(out) {}

void CodeGeneration::Do()
{
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

string CodeGeneration::getOperand(Variable *var)
{
    // prosuta promenljiva uvek "zivi" privremeno u rezervisanom $t3
    if (var->isSpilled())
        return "$t3";

    return getRegisterName(var);
}

void CodeGeneration::emitSpillLoad(ofstream &out, Variable *var)
{
    if (var->isSpilled())
        out << "\tlw\t$t3, " << var->getSpillLabel() << endl;
}

void CodeGeneration::emitSpillStore(ofstream &out, Variable *var)
{
    if (var->isSpilled())
        out << "\tsw\t$t3, " << var->getSpillLabel() << endl;
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

    // rezervisi memorijski slot za svaku prosutu (spilled) registarsku promenljivu
    for (Variable *var : variables)
    {
        if (var->getType() == Variable::REG_VAR && var->isSpilled())
            out << var->getSpillLabel() << ":\t.word 0" << endl;
    }

    out << endl;
}

void CodeGeneration::generateText(ofstream &out)
{
    out << ".text" << endl;
    out << function << ":" << endl;

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

            emitSpillLoad(out, src1);
            emitSpillLoad(out, src2);

            out << "\tadd\t" << getOperand(dst) << ", "
                << getOperand(src1) << ", " << getOperand(src2) << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_ADDI:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            emitSpillLoad(out, src1);

            out << "\taddi\t" << getOperand(dst) << ", "
                << getOperand(src1) << ", " << instr->getOffset() << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_SUB:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            emitSpillLoad(out, src1);
            emitSpillLoad(out, src2);

            out << "\tsub\t" << getOperand(dst) << ", "
                << getOperand(src1) << ", " << getOperand(src2) << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_LA:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            out << "\tla\t" << getOperand(dst) << ", " << src1->getName() << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_LI:
        {
            Variable *dst = instr->getDst().front();

            out << "\tli\t" << getOperand(dst) << ", " << instr->getOffset() << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_LW:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();

            emitSpillLoad(out, src1);

            out << "\tlw\t" << getOperand(dst) << ", " << instr->getOffset()
                << "(" << getOperand(src1) << ")" << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_SW:
        {
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *base = *it;

            emitSpillLoad(out, src1);
            emitSpillLoad(out, base);

            out << "\tsw\t" << getOperand(src1) << ", " << instr->getOffset()
                << "(" << getOperand(base) << ")" << endl;
            break;
        }
        case I_BLTZ:
        {
            Variable *src1 = instr->getSrc().front();

            emitSpillLoad(out, src1);

            out << "\tbltz\t" << getOperand(src1) << ", "
                << instr->getBranchLabel() << endl;
            break;
        }
        case I_B:
            out << "\tb\t" << instr->getBranchLabel() << endl;
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

            emitSpillLoad(out, src1);
            emitSpillLoad(out, src2);

            out << "\tmul\t" << getOperand(dst) << ", "
                << getOperand(src1) << ", " << getOperand(src2) << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_AND:
        {
            Variable *dst = instr->getDst().front();
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            emitSpillLoad(out, src1);
            emitSpillLoad(out, src2);

            out << "\tand\t" << getOperand(dst) << ", "
                << getOperand(src1) << ", " << getOperand(src2) << endl;

            emitSpillStore(out, dst);
            break;
        }
        case I_BEQ:
        {
            Variable *src1 = instr->getSrc().front();
            auto it = instr->getSrc().begin();
            ++it;
            Variable *src2 = *it;

            emitSpillLoad(out, src1);
            emitSpillLoad(out, src2);

            out << "\tbeq\t" << getOperand(src1) << ", "
                << getOperand(src2) << ", " << instr->getBranchLabel() << endl;
            break;
        }
        default:
            break;
        }
    }
}