#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class SymbolTable {
private:
    std::unordered_map<std::string, int> table;
    std::vector<std::string> order; // preserve insertion order for printing

public:
    void setSymbol(const std::string &symbol, int address) {
        if (table.find(symbol) == table.end()) {
            table[symbol] = address;
            order.push_back(symbol);
        } else {
            // Update address if redefined
            table[symbol] = address;
        }
    }

    bool find(const std::string &symbol) const {
        return table.find(symbol) != table.end();
    }

    int getSymbolAddress(const std::string &symbol) const {
        auto it = table.find(symbol);
        if (it != table.end()) {
            return it->second;
        }
        return -1; // undefined
    }

    int getSymbolIndex(const std::string &symbol) const {
        for (size_t i = 0; i < order.size(); i++) {
            if (order[i] == symbol) return static_cast<int>(i);
        }
        return -1; // not found
    }

    void print() const {
        for (size_t i = 0; i < order.size(); i++) {
            std::cout << i << "\t" << order[i] << "\t" << table.at(order[i]) << "\n";
        }
    }

    void saveTable(const std::string &filepath) const {
        std::ofstream out(filepath);
        for (size_t i = 0; i < order.size(); i++) {
            out << i << "\t" << order[i] << "\t" << table.at(order[i]) << "\n";
        }
    }
};
