#pragma once
#include <string>
#include <unordered_map>

enum class MnemonicClass {
    IS, DL, AD, INVALID
};

struct Mnemonic {
    MnemonicClass mclass;
    int opCode;

    std::string getClass() const;
};

class MnemonicTable {
private:
    std::unordered_map<std::string, Mnemonic> table;

public:
    MnemonicTable();
    Mnemonic getMnemonic(const std::string &name);
};
