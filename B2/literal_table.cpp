#include "literal_table.h"
#include <iostream>
#include <fstream>

void LiteralTable::setLiteral(const std::string &literal, int address) {
    for (auto &entry : literals)
        if (entry.value == literal) return;
    literals.push_back({literal, address});
}

int LiteralTable::getLiteralIndex(const std::string &literal) const {
    for (size_t i = 0; i < literals.size(); i++)
        if (literals[i].value == literal) return static_cast<int>(i);
    return -1;
}

void LiteralTable::initializeLiterals(int &locCounter) {
    for (auto &entry : literals)
        if (entry.address == -1) entry.address = locCounter++;
}

void LiteralTable::newPool() {
    poolTable.push_back(static_cast<int>(literals.size()));
}

void LiteralTable::print() const {
    std::cout << "Index\tLiteral\tAddress\n";
    for (size_t i = 0; i < literals.size(); i++)
        std::cout << i << "\t" << literals[i].value << "\t" << literals[i].address << "\n";

    std::cout << "Pool Table: ";
    for (auto index : poolTable) std::cout << index << " ";
    std::cout << "\n";
}

void LiteralTable::saveTable(const std::string &filepath) const {
    std::ofstream out(filepath);
    if (out.is_open()) {
        for (size_t i = 0; i < literals.size(); i++)
            out << i << "\t" << literals[i].value << "\t" << literals[i].address << "\n";
        out.close();
    }
}

int LiteralTable::getLiteralAddressByIndex(int index) const {
    return (index >= 0 && index < static_cast<int>(literals.size())) ?
        literals[index].address : -1;
}
