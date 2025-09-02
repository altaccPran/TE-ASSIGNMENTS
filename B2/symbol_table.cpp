#include "symbol_table.h"
#include <iostream>
#include <fstream>

void SymbolTable::setSymbol(const std::string &symbol, int address) {
    if (table.find(symbol) == table.end()) {
        table[symbol] = address;
        order.push_back(symbol);
    } else {
        table[symbol] = address;
    }
}

bool SymbolTable::find(const std::string &symbol) const {
    return table.find(symbol) != table.end();
}

int SymbolTable::getSymbolAddress(const std::string &symbol) const {
    auto it = table.find(symbol);
    return (it != table.end()) ? it->second : -1;
}

int SymbolTable::getSymbolIndex(const std::string &symbol) const {
    for (size_t i = 0; i < order.size(); i++)
        if (order[i] == symbol) return static_cast<int>(i);
    return -1;
}

int SymbolTable::getSymbolAddressByIndex(int index) const {
    return (index >= 0 && index < static_cast<int>(order.size())) ?
        table.at(order[index]) : -1;
}

void SymbolTable::print() const {
    std::cout << "Index\tSymbol\tAddress\n";
    for (size_t i = 0; i < order.size(); i++)
        std::cout << i << "\t" << order[i] << "\t" << table.at(order[i]) << "\n";
}

void SymbolTable::saveTable(const std::string &filepath) const {
    std::ofstream out(filepath);
    if (out.is_open()) {
        for (size_t i = 0; i < order.size(); i++)
            out << i << "\t" << order[i] << "\t" << table.at(order[i]) << "\n";
        out.close();
    }
}
