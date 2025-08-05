#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class LiteralTable {
private:
    struct LiteralEntry {
        std::string value;
        int address;
    };

    std::vector<LiteralEntry> literals;
    std::vector<int> poolTable; // starting index of each literal pool

public:
    void setLiteral(const std::string &literal, int address) {
        // Check if literal already exists
        for (auto &entry : literals) {
            if (entry.value == literal) return;
        }
        literals.push_back({literal, address});
    }

    int getLiteralIndex(const std::string &literal) const {
        for (size_t i = 0; i < literals.size(); i++) {
            if (literals[i].value == literal) return static_cast<int>(i);
        }
        return -1;
    }

    void initializeLiterals(int &locCounter) {
        for (auto &entry : literals) {
            if (entry.address == -1) { // assign only unassigned literals
                entry.address = locCounter++;
            }
        }
    }

    void newPool() {
        poolTable.push_back(static_cast<int>(literals.size()));
    }

    void print() const {
        std::cout << "Index\tLiteral\tAddress\n";
        for (size_t i = 0; i < literals.size(); i++) {
            std::cout << i << "\t" << literals[i].value << "\t" << literals[i].address << "\n";
        }

        std::cout << "Pool Table: ";
        for (auto index : poolTable) {
            std::cout << index << " ";
        }
        std::cout << "\n";
    }

    void saveTable(const std::string &filepath) const {
        std::ofstream out(filepath);
        for (size_t i = 0; i < literals.size(); i++) {
            out << i << "\t" << literals[i].value << "\t" << literals[i].address << "\n";
        }
    }
};
