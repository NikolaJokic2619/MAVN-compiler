#pragma once

#include <string>

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

class SyntaxAnalysis
{
public:
    /**
     * Constructor
     */
    SyntaxAnalysis(TokenList &tokenList);
    ~SyntaxAnalysis();

    /**
     * Method which performs lexical analysis
     */
    bool Do();

    Variables &getVariables() { return variables; }
    Instructions &getInstructions() { return instructions; }
    std::string getFunctionName() { return functionName; }

    void printSyntaxError(Token token);

private:
    void Q();
    void S();
    void L();
    void E();

    /**
     * Eats the current token if its type is "t"
     * otherwise reports syntax error
     *
     * param[in] - t - the expected token type
     */
    void eat(TokenType t);

    /**
     * Current token type
     */
    TokenType currentType();

    /**
     * Current token value
     */
    std::string currentValue();

    Variable *findVariable(const std::string &name);

    TokenList &tokenList;
    TokenList::iterator currentToken;

    bool errorFound;
    Token errorToken;

    Variables variables;
    Instructions instructions;

    int instructionPosition;
    std::string functionName;
    std::string nextLabel;
};