#pragma once

#include "Types.h"

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
    enum VariableType
    {
        MEM_VAR,
        REG_VAR,
        NO_TYPE
    };

    Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), m_value(0) {}
    Variable(std::string name, int pos, VariableType type, int value = 0)
        : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign), m_value(value) {}

    // Getters
    VariableType getType() { return m_type; }
    std::string getName() { return m_name; }
    int getPosition() { return m_position; }
    Regs getAssignment() { return m_assignment; }
    int getValue() { return m_value; }

    // Seters
    void setType(VariableType t) { m_type = t; }
    void setName(std::string n) { m_name = n; }
    void setPosition(int p) { m_position = p; }
    void setAssignment(Regs r) { m_assignment = r; }
    void setValue(int v) { m_value = v; }

private:
    VariableType m_type;
    std::string m_name;
    int m_position;
    Regs m_assignment;
    int m_value;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable *> Variables;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
    Instruction() : m_position(0), m_type(I_NO_TYPE) {}
    Instruction(int pos, InstructionType type, Variables &dst, Variables &src) : m_position(pos), m_type(type), m_dst(dst), m_src(src) {}

    // Getteres
    int getPosition() { return m_position; }
    InstructionType getType() { return m_type; }
    Variables &getDst() { return m_dst; }
    Variables &getSrc() { return m_src; }
    Variables &getUse() { return m_use; }
    Variables &getDef() { return m_def; }
    Variables &getIn() { return m_in; }
    Variables &getOut() { return m_out; }
    int getOffset() { return m_offset; }
    std::string getLabelName() { return m_labelName; }
    std::string getBranchLabel() { return m_branchLabel; }

    std::list<Instruction *> &getSucc() { return m_succ; }
    std::list<Instruction *> &getPred() { return m_pred; }

    // Setters
    void setPosition(int p) { m_position = p; }
    void setType(InstructionType t) { m_type = t; }
    void setOffset(int o) { m_offset = o; }
    void setLabelName(std::string name) { m_labelName = name; }
    void setBranchLabel(std::string label) { m_branchLabel = label; }

private:
    int m_position;
    InstructionType m_type;

    Variables m_dst;
    Variables m_src;

    Variables m_use;
    Variables m_def;
    Variables m_in;
    Variables m_out;
    std::list<Instruction *> m_succ;
    std::list<Instruction *> m_pred;

    int m_offset;
    std::string m_labelName;
    std::string m_branchLabel;
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction *> Instructions;
