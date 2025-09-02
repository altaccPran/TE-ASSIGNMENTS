#pragma once
#include <string>
#include <vector>
#include "symbol_table.h"
#include "literal_table.h"
#include "mnemonic_table.h"

class AssemblerPass2 {
private:
    SymbolTable symbolTable;
    LiteralTable literalTable;
    MnemonicTable mnemonicTable;
    std::string icFilepath;
    std::string symbolTableFilepath;
    std::string literalTableFilepath;
    std::string machineCodeFilepath;

    void loadSymbolTable();
    void loadLiteralTable();
    static std::vector<std::string> tokenize(const std::string &line);
    static void printFile(const std::string &filepath);

public:
    AssemblerPass2(
        const std::string &icFilepath,
        const std::string &symbolTableFilepath,
        const std::string &literalTableFilepath,
        const std::string &machineCodeFilepath
    );
    void performPass();
};
