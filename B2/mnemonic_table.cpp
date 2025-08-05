#include <unordered_map>
#include <string>

enum class MnemonicClass {
    IS, // Imperative Statement
    DL, // Declarative Statement
    AD, // Assembler Directive
    INVALID
};

struct Mnemonic {
    MnemonicClass mclass;
    int opCode;
    
    std::string getClass() const {
        switch (mclass) {
            case MnemonicClass::IS: return "IS";
            case MnemonicClass::DL: return "DL";
            case MnemonicClass::AD: return "AD";
            default: return "??";
        }
    }
};

class MnemonicTable {
private:
    std::unordered_map<std::string, Mnemonic> table;

public:
    MnemonicTable() {
        // Imperative Statements (IS)
        table["STOP"]  = {MnemonicClass::IS, 0};
        table["ADD"]   = {MnemonicClass::IS, 1};
        table["SUB"]   = {MnemonicClass::IS, 2};
        table["MULT"]  = {MnemonicClass::IS, 3};
        table["MOVER"] = {MnemonicClass::IS, 4};
        table["MOVEM"] = {MnemonicClass::IS, 5};
        table["COMP"]  = {MnemonicClass::IS, 6};
        table["BC"]    = {MnemonicClass::IS, 7};
        table["DIV"]   = {MnemonicClass::IS, 8};
        table["READ"]  = {MnemonicClass::IS, 9};
        table["PRINT"] = {MnemonicClass::IS, 10};

        // Declarative Statements (DL)
        table["DC"] = {MnemonicClass::DL, 1};
        table["DS"] = {MnemonicClass::DL, 2};

        // Assembler Directives (AD)
        table["START"] = {MnemonicClass::AD, 1};
        table["END"]   = {MnemonicClass::AD, 2};
        table["ORIGIN"]= {MnemonicClass::AD, 3};
        table["EQU"]   = {MnemonicClass::AD, 4};
        table["LTORG"] = {MnemonicClass::AD, 5};
    }

    Mnemonic getMnemonic(const std::string &name) {
        auto it = table.find(name);
        if (it != table.end()) {
            return it->second;
        }
        return {MnemonicClass::INVALID, -1};
    }
};
