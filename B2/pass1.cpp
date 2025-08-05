#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "tokenizer.cpp"
#include "symbol_table.cpp"
#include "literal_table.cpp"
#include "mnemonic_table.cpp"

using std::fstream;
using std::ios;
using std::string;
using std::vector;
using std::unordered_map;
using std::stringstream;
using std::cout;

#define LBR '\n'

class AssemblerPass1 {

    unordered_map<string,int> conditionCodes = {
        { "LT",1 }, { "LE",2 }, { "EQ",3 },
        { "GT",4 }, { "GE",5 }, { "ANY",6 }
    };

    unordered_map<string,int> registerCodes = {
        { "AREG",1 }, { "BREG",2 }, { "CREG",3 }, { "DREG",4 }
    };

    vector<vector<string>> instruction_tokens;
    fstream icOutputStream;
    SymbolTable symbolTable;
    Tokenizer tokenizer;
    MnemonicTable mnemonicTable;
    LiteralTable literalTable;

    string symbolTableFilepath; 
    string literalTableFilepath; 
    string icFilepath;

public:
    AssemblerPass1(
        const string& sourceFilepath, 
        const string& icFilepath,
        const string& symbolTableFilepath,
        const string& literalTableFilepath
    ) {
        this->icFilepath = icFilepath; 
        this->symbolTableFilepath = symbolTableFilepath; 
        this->literalTableFilepath = literalTableFilepath;   

        string sourceContents = readTextFile(sourceFilepath);
        vector<string> lines = readLines(sourceContents); 
        for (const string& line : lines) {
            instruction_tokens.push_back(tokenizer.getTokens(line));
        }

        icOutputStream.open(icFilepath, ios::out); 
    }

    void performPass() {
        int locationCounter = 0; 
        literalTable.newPool(); 

        for (auto &instruction : instruction_tokens) {
            string label = instruction[0];
            string mnemonic_string = instruction[1];
            string op1 = instruction[2];
            string op2 = instruction[3];

            if (mnemonic_string.empty()) continue;

            Mnemonic mnemonic = mnemonicTable.getMnemonic(mnemonic_string);

            // Store label if present
            if (!label.empty()) {
                symbolTable.setSymbol(label, locationCounter);
            }

            // START
            if (mnemonic_string == "START") {
                locationCounter = stoi(op1); 
                icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ") (C," 
                               << op1 << ")" << LBR; 
            }

            // ORIGIN
            else if (mnemonic_string == "ORIGIN") {
                if (checkSymbolOffsetExpr(op1)) {
                    string symbol; 
                    locationCounter = evaluateSymbolOffsetExpr(op1, symbol);
                    icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ") (S," 
                                   << symbolTable.getSymbolIndex(symbol) << ")" << LBR; 
                } else {
                    locationCounter = symbolTable.find(op1) ? 
                                      symbolTable.getSymbolAddress(op1) : stoi(op1);
                    icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ") (C," 
                                   << locationCounter << ")" << LBR; 
                }
            }

            // LTORG
            else if (mnemonic_string == "LTORG") {
                literalTable.initializeLiterals(locationCounter); 
                icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ")" << LBR;
            }

            // EQU
            else if (mnemonic_string == "EQU") {
                int address;
                if (checkSymbolOffsetExpr(op1)) {
                    string symbol; 
                    address = evaluateSymbolOffsetExpr(op1, symbol); 
                } else {
                    address = symbolTable.find(op1) ?
                              symbolTable.getSymbolAddress(op1) : stoi(op1);
                }
                symbolTable.setSymbol(label, address); 
                icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ") (C," 
                               << address << ")" << LBR; 
            }

            // END
            else if (mnemonic_string == "END") {
                literalTable.initializeLiterals(locationCounter); 
                icOutputStream << " (" << mnemonic.getClass() << "," << mnemonic.opCode << ")" << LBR;
                break;
            }

            // Imperative Statements
            else if (mnemonic.mclass == MnemonicClass::IS) {
                if (mnemonic_string == "READ" || mnemonic_string == "PRINT") {
                    if (!symbolTable.find(op1)) symbolTable.setSymbol(op1, -1);
                    icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                                   << mnemonic.opCode << ") (S," 
                                   << symbolTable.getSymbolIndex(op1) << ")" << LBR; 
                }
                else if (mnemonic_string == "BC") {
                    if (!symbolTable.find(op2)) symbolTable.setSymbol(op2, -1);
                    icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                                   << mnemonic.opCode << ") (" << conditionCodes[op1] 
                                   << ") (S," << symbolTable.getSymbolIndex(op2) << ")" << LBR;
                }
                else if (mnemonic_string == "STOP") {
                    icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                                   << mnemonic.opCode << ")" << LBR;
                }
                else { // MOVEM, MOVER, ADD, SUB etc.
                    if (!op2.empty() && op2[0] == '=') {
                        string literal = extractLiteralValue(op2);
                        literalTable.setLiteral(literal, -1);
                        icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                                       << mnemonic.opCode << ") (" << registerCodes[op1] 
                                       << ") (L," << literalTable.getLiteralIndex(literal) << ")" << LBR;
                    } else {
                        if (!symbolTable.find(op2)) symbolTable.setSymbol(op2, -1);
                        icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                                       << mnemonic.opCode << ") (" << registerCodes[op1] 
                                       << ") (S," << symbolTable.getSymbolIndex(op2) << ")" << LBR;
                    }
                }
                locationCounter++; 
            }

            // Declarative Statements
            else if (mnemonic.mclass == MnemonicClass::DL) {
                symbolTable.setSymbol(label, locationCounter);
                icOutputStream << locationCounter << " (" << mnemonic.getClass() << "," 
                               << mnemonic.opCode << ") (C," << op1 << ")" << LBR;
                locationCounter += (mnemonic_string == "DS") ? stoi(op1) : 1;
            }
        }

        icOutputStream.close(); 

        cout << "------ Intermediate Code ------" << LBR;
        printIC(); 

        cout << "------ Symbol Table ------" << LBR; 
        symbolTable.print(); 

        cout << "------ Literal Table ------" << LBR; 
        literalTable.print(); 

        saveTables(symbolTable, literalTable);
    }

private:
    bool checkSymbolOffsetExpr(const string& expr) {
        return (expr.find("+") != string::npos) || (expr.find("-") != string::npos); 
    }

    int evaluateSymbolOffsetExpr(const string& expr, string& symbolName) {
        size_t pos = expr.find_first_of("+-");
        symbolName = expr.substr(0, pos);
        int offset = std::stoi(expr.substr(pos));
        return symbolTable.getSymbolAddress(symbolName) + offset;
    }

    string extractLiteralValue(const string& literalToken) {
        size_t firstQuote = literalToken.find('\'');
        size_t lastQuote = literalToken.rfind('\'');
        if (firstQuote != string::npos && lastQuote != string::npos && lastQuote > firstQuote)
            return literalToken.substr(firstQuote + 1, lastQuote - firstQuote - 1);
        return literalToken.substr(1);
    }

    void printIC() {
        string sourceContents = readTextFile(icFilepath);
        cout << sourceContents << LBR; 
    }

    static string readTextFile(const string& filepath) {
        std::ifstream inputStream(filepath);
        if (!inputStream.is_open()) return "";
        std::stringstream buffer;
        buffer << inputStream.rdbuf();
        return buffer.str();
    }

    static vector<string> readLines(const string& contents) {
        vector<string> lines;
        stringstream ss(contents);
        string line;
        while (getline(ss, line)) lines.push_back(line);
        return lines;
    }

    void saveTables(const SymbolTable& sym, const LiteralTable& lit) {
        sym.saveTable(symbolTableFilepath); 
        lit.saveTable(literalTableFilepath);
    }
};
