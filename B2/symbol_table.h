#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable {
private:
    std::unordered_map<std::string, int> table;
    std::vector<std::string> order;

public:
    void setSymbol(const std::string &symbol, int address);
    bool find(const std::string &symbol) const;
    int getSymbolAddress(const std::string &symbol) const;
    int getSymbolIndex(const std::string &symbol) const;
    int getSymbolAddressByIndex(int index) const;
    void print() const;
    void saveTable(const std::string &filepath) const;
};
