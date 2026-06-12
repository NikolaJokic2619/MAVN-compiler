#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "ControlFlowGraph.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"

#include <iostream>
#include <exception>

using namespace std;

int main()
{
    try
    {
        std::string fileName = "../examples/simple.mavn";
        bool retVal = false;

        LexicalAnalysis lex;

        if (!lex.readInputFile(fileName))
            throw runtime_error("\nException! Failed to open input file!\n");

        lex.initialize();

        retVal = lex.Do();

        if (retVal)
        {
            cout << "Lexical analysis finished successfully!" << endl;
            lex.printTokens();
        }
        else
        {
            lex.printLexError();
            throw runtime_error("\nException! Lexical analysis failed!\n");
        }

        SyntaxAnalysis syn(lex.getTokenList());

        retVal = syn.Do();

        if (retVal)
        {
            cout << "Syntax analysis finished successfully!" << endl;
        }
        else
        {
            syn.printSyntaxError(syn.getErrorToken());
            throw runtime_error("\nException! Syntax analysis failed!\n");
        }

        // Syntax Anlysis result
        cout << "\nFunction: " << syn.getFunctionName() << endl;

        cout << syn.getInstructions().size() << " Instructions" << endl;

        cout << syn.getVariables().size() << " Variables" << endl;

        cout << "\nVariables: " << endl;

        for (Variable *v : syn.getVariables())
        {
            cout << v->getName();
            if (v->getType() == Variable::MEM_VAR)
            {
                cout << " mem = " << v->getValue() << endl;
            }
            else
            {
                cout << " reg" << endl;
            }
        }

        // Control Graph Flow
        ControlFlowGraph cfg(syn.getInstructions());
        cfg.Do();
        cfg.printGraph();

        // Liveness Analysis
        LivenessAnalysis liveness(syn.getInstructions());
        liveness.Do();
        liveness.printSets();

        // Interference Graph
        InterferenceGraph ig(syn.getVariables(), syn.getInstructions());
        ig.Do();
        ig.printGraph();
    }
    catch (runtime_error e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
