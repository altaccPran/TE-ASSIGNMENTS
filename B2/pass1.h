#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "tokenizer.h"
#include "symbol_table.h"
#include "literal_table.h"
#include "mnemonic_table.h"

class AssemblerPass1 {
private:
    std::unordered_map<std::string,int> conditionCodes;
    std::unordered_map<std::string,int> registerCodes;
    std::vector<std::vector<std::string>> instruction_tokens;
    std::ofstream icOutputStream;
    SymbolTable symbolTable;
    Tokenizer tokenizer;
    MnemonicTable mnemonicTable;
    LiteralTable literalTable;
    std::string symbolTableFilepath; 
    std::string literalTableFilepath; 
    std::string icFilepath;

    bool checkSymbolOffsetExpr(const std::string& expr);
    int evaluateSymbolOffsetExpr(const std::string& expr, std::string& symbolName);
    std::string extractLiteralValue(const std::string& literalToken);
    void printIC();
    static std::string readTextFile(const std::string& filepath);
    static std::vector<std::string> readLines(const std::string& contents);
    void saveTables(const SymbolTable& sym, const LiteralTable& lit);

public:
    AssemblerPass1(
        const std::string& sourceFilepath, 
        const std::string& icFilepath,
        const std::string& symbolTableFilepath,
        const std::string& literalTableFilepath
    );
    void performPass();
};
