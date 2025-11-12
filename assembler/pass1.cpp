#include <bits/stdc++.h>
using namespace std;

struct Symbol { int index; string name; int address; };
struct Literal { int index; string literal; int address; };

int main() {
    ifstream fin("input.asm");
    ofstream ic("intermediatecode.txt"), sym("symbtab.txt"), lit("literaltab.txt"), pool("pooltable.txt");
    if(!fin){ cout<<"Input file not found!\n"; return 0; }

    map<string,pair<string,string>> MOT = {
        {"STOP",{"IS","00"}},{"ADD",{"IS","01"}},{"SUB",{"IS","02"}},
        {"MULT",{"IS","03"}},{"MOVER",{"IS","04"}},{"MOVEM",{"IS","05"}},
        {"COMP",{"IS","06"}},{"BC",{"IS","07"}},{"DIV",{"IS","08"}},
        {"READ",{"IS","09"}},{"PRINT",{"IS","10"}}
    };

    map<string,string> AD = {{"START","01"},{"END","02"},{"ORIGIN","03"},{"EQU","04"},{"LTORG","05"}};
    map<string,string> DL = {{"DC","01"},{"DS","02"}};
    map<string,int> REG = {{"AREG",1},{"BREG",2},{"CREG",3},{"DREG",4}};

    vector<Symbol> SYMTAB; 
    vector<Literal> LITTAB;
    vector<int> POOLTAB;

    int LC=0, symIndex=1, litIndex=1;
    POOLTAB.push_back(0);

    string line;
    while(getline(fin,line)){
        if(line.empty()) continue;
        stringstream ss(line); 
        vector<string> tokens; 
        string token;
        while(ss>>token) tokens.push_back(token); 
        if(tokens.empty()) continue;

        int startIndex=0;

        // Check if it's EQU directive

        //LABEL EQU (SYMBOL+5)--->expr
       if (tokens.size() >= 3 && tokens[1] == "EQU") {
            string label = tokens[0];
            string expr = tokens[2];
            int newAddr = 0;

            // Case 1: Expression contains '+' or '-'
            if (expr.find('+') != string::npos || expr.find('-') != string::npos) {
                string sym = expr.substr(0, expr.find_first_of("+-"));
                char op = expr[expr.find_first_of("+-")];
                int val = stoi(expr.substr(expr.find_first_of("+-") + 1));

                for (auto &s : SYMTAB) {
                    if (s.name == sym) {
                        if (op == '+') {
                            newAddr = s.address + val;
                        } else {
                            newAddr = s.address - val;
                        }
                        break;
                    }
                }
            } 
            // Case 2: Expression is just a symbol
            else {
                for (auto &s : SYMTAB) {
                    if (s.name == expr) {
                        newAddr = s.address;
                        break;
                    }
                }
            }

            // Add or update label in SYMTAB
            bool exists = false;
            for (auto &s : SYMTAB) {
                if (s.name == label) {
                    s.address = newAddr;
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                SYMTAB.push_back({symIndex++, label, newAddr});
            }
            ic << setw(3) << LC << "\t(AD,04)\t(C," << newAddr << ")\n";
            continue;
        }


        // Label (skip registers/literals)
        if (MOT.find(tokens[0]) == MOT.end() && AD.find(tokens[0]) == AD.end() &&
            DL.find(tokens[0]) == DL.end() && REG.find(tokens[0]) == REG.end() &&
            tokens[0][0] != '=' && tokens[0] != "EQU") {

            string label = tokens[0];
            bool exists = false;

            for (auto &s : SYMTAB) {
                if (s.name == label) {
                    s.address = LC;
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                SYMTAB.push_back({symIndex++, label, LC});
            }

            startIndex = 1;
        }


        if(startIndex>=tokens.size()) continue;
        string opcode=tokens[startIndex];

        // AD
        if (AD.find(opcode) != AD.end()) {
            if (opcode == "START") {
                LC = stoi(tokens[startIndex + 1]);
                ic << setw(3) << LC << "\t(AD,01)\t(C," << LC << ")\n";
            }
            else if (opcode == "END" || opcode == "LTORG") {
                // Assign LC to literals in current pool and generate DL statements
                for (int i = POOLTAB.back(); i < LITTAB.size(); i++) {
                    if (LITTAB[i].address == -1) {
                        LITTAB[i].address = LC;
                        ic << setw(3) << LC << "\t(DL,01)\t(L," << LITTAB[i].index << ")\n";
                        LC++;
                    }
                }
                if (LITTAB.size() != POOLTAB.back()) {
                    POOLTAB.push_back(LITTAB.size());
                }
                ic << setw(3) << LC << "\t(AD," << AD[opcode] << ")\n";
            }
            else if (opcode == "ORIGIN") {
                string expr = tokens[startIndex + 1];
                int newLC = 0;

                if (expr.find('+') != string::npos || expr.find('-') != string::npos) {
                    string sym = expr.substr(0, expr.find_first_of("+-"));
                    char op = expr[expr.find_first_of("+-")];
                    int val = stoi(expr.substr(expr.find_first_of("+-") + 1));

                    for (auto &s : SYMTAB) {
                        if (s.name == sym) {
                            if (op == '+') {
                                newLC = s.address + val;
                            } else {
                                newLC = s.address - val;
                            }
                            break;
                        }
                    }
                }
                else {
                    for (auto &s : SYMTAB) {
                        if (s.name == expr) {
                            newLC = s.address;
                            break;
                        }
                    }
                }

                ic << setw(3) << LC << "\t(AD,03)\t(C," << newLC << ")\n";
                LC = newLC;
            }
            continue;
        }


        // DL
        if (DL.find(opcode) != DL.end()) {
            ic << setw(3) << LC << "\t(DL," << DL[opcode] << ")";

            if (opcode == "DC") {
                ic << "\t(C," << tokens[startIndex + 1] << ")";
                LC = LC + 1;
            }
            else if (opcode == "DS") {
                ic << "\t(C," << tokens[startIndex + 1] << ")";
                LC = LC + stoi(tokens[startIndex + 1]);
            }

            ic << "\n";
            continue; 
        }

        // MOT
        if (MOT.find(opcode) != MOT.end()) {
            ic << setw(3) << LC << "\t(" << MOT[opcode].first << "," << MOT[opcode].second << ")\t";

            for (int i = startIndex + 1; i < tokens.size(); i++) {
                string opnd = tokens[i];

                // Remove trailing comma
                if (!opnd.empty() && opnd.back() == ',') {
                    opnd.pop_back();
                }

                // Check if operand is a register
                if (REG.find(opnd) != REG.end()) {
                    ic << "(R," << REG[opnd] << ")\t";
                }
                // Check if operand is a literal
                else if (!opnd.empty() && opnd[0] == '=') {
                    bool exists = false;

                    for (auto &l : LITTAB) {
                        if (l.literal == opnd) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        LITTAB.push_back({litIndex++, opnd, -1});
                    }
                    int idx = 0;
                    for (auto &l : LITTAB) {
                        if (l.literal == opnd) {
                            idx = l.index;
                            break;
                        }
                    }
                    ic << "(L," << idx << ")\t";
                }
                // Otherwise, operand is a symbol
                else {
                    int idx = 0;
                    bool exists = false;

                    for (auto &s : SYMTAB) {
                        if (s.name == opnd) {
                            exists = true;
                            idx = s.index;
                            break;
                        }
                    }
                    if (!exists) {
                        SYMTAB.push_back({symIndex++, opnd, -1});
                        idx = SYMTAB.back().index;
                    }
                    ic << "(S," << idx << ")\t";
                }
            }
            ic << "\n";
            LC = LC + 1;
        }
    }

    fin.close(); ic.close();

    // SYMTAB
    for(auto &s:SYMTAB) sym<<s.index<<"\t"<<s.name<<"\t"<<s.address<<"\n";
    sym.close();

    // LITTAB
    for(auto &l:LITTAB) lit<<l.index<<"\t"<<l.literal<<"\t"<<l.address<<"\n";
    lit.close();

    // POOLTAB
    for(int i=0;i<POOLTAB.size();i++) pool<<"#"<<i+1<<"\t"<<POOLTAB[i]+1<<"\n";
    pool.close();

    cout<<"Pass-I completed successfully.\n";
    return 0;
}