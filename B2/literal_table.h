#pragma once
#include <string>
#include <vector>

class LiteralTable {
private:
    struct LiteralEntry {
        std::string value;
        int address;
    };

    std::vector<LiteralEntry> literals;
    std::vector<int> poolTable;

public:
    void setLiteral(const std::string &literal, int address);
    int getLiteralIndex(const std::string &literal) const;
    void initializeLiterals(int &locCounter);
    void newPool();
    void print() const;
    void saveTable(const std::string &filepath) const;
    int getLiteralAddressByIndex(int index) const;
};
