#include "pass2.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cout;

AssemblerPass2::AssemblerPass2(
    const string &icFilepath,
    const string &symbolTableFilepath,
    const string &literalTableFilepath,
    const string &machineCodeFilepath
) {
    this->icFilepath = icFilepath;
    this->symbolTableFilepath = symbolTableFilepath;
    this->literalTableFilepath = literalTableFilepath;
    this->machineCodeFilepath = machineCodeFilepath;
}

void AssemblerPass2::performPass() {
    loadSymbolTable();
    loadLiteralTable();

    ifstream ic(icFilepath);
    if (!ic.is_open()) {
        cout << "Error: Could not open intermediate code file " << icFilepath << std::endl;
        return;
    }

    ofstream mc(machineCodeFilepath);
    if (!mc.is_open()) {
        cout << "Error: Could not create machine code file " << machineCodeFilepath << std::endl;
        return;
    }

    string line;
    while (getline(ic, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        ss >> token;

        // Skip non-location lines
        int loc = -1;
        try {
            loc = std::stoi(token);
        } catch (...) {
            // Not a location counter line, skip processing
            continue;
        }

        if (loc != -1) {
            mc << loc << "\t";
            // Get the rest of the line after location
            string restOfLine = line.substr(token.length());
            size_t firstNonSpace = restOfLine.find_first_not_of(" \t");
            if (firstNonSpace != string::npos) {
                line = restOfLine.substr(firstNonSpace);
            }
        }

        vector<string> tokens = tokenize(line);
        string output;

        for (auto &tok : tokens) {
            if (tok.length() >= 2 && tok[0] == '(' && tok.back() == ')') {
                string inner = tok.substr(1, tok.size() - 2);
                stringstream innerSS(inner);
                string type;
                getline(innerSS, type, ',');
                string value;
                getline(innerSS, value);

                if (type == "IS") {
                    output += value + "\t";
                } else if (type == "C") {
                    output += value + "\t";
                } else if (type == "S") {
                    int symbolIndex = std::stoi(value);
                    int address = symbolTable.getSymbolAddressByIndex(symbolIndex);
                    output += std::to_string(address) + "\t";
                } else if (type == "L") {
                    int literalIndex = std::stoi(value);
                    int address = literalTable.getLiteralAddressByIndex(literalIndex);
                    output += std::to_string(address) + "\t";
                }
            } else if (!tok.empty() && tok.find(',') == string::npos && 
                      tok.find('(') == string::npos && tok.find(')') == string::npos) {
                // Plain register codes or condition codes
                try {
                    int code = std::stoi(tok);
                    output += std::to_string(code) + "\t";
                } catch (...) {
                    // Not a number, skip
                }
            }
        }

        // Remove trailing tab
        if (!output.empty() && output.back() == '\t') {
            output.pop_back();
        }

        mc << output << "\n";
    }

    ic.close();
    mc.close();

    cout << "------ Machine Code ------\n";
    printFile(machineCodeFilepath);
}

void AssemblerPass2::loadSymbolTable() {
    ifstream file(symbolTableFilepath);
    if (!file.is_open()) {
        cout << "Error: Could not open symbol table file " << symbolTableFilepath << std::endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int idx, address;
        string symbol;
        if (ss >> idx >> symbol >> address) {
            symbolTable.setSymbol(symbol, address);
        }
    }
    file.close();
}

void AssemblerPass2::loadLiteralTable() {
    ifstream file(literalTableFilepath);
    if (!file.is_open()) {
        cout << "Error: Could not open literal table file " << literalTableFilepath << std::endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int idx, address;
        string value;
        if (ss >> idx >> value >> address) {
            literalTable.setLiteral(value, address);
        }
    }
    file.close();
}

vector<string> AssemblerPass2::tokenize(const string &line) {
    stringstream ss(line);
    string token;
    vector<string> tokens;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void AssemblerPass2::printFile(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filepath << std::endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}
