#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream ic("intermediatecode.txt");
    ifstream symtab("symbtab.txt");
    ifstream littab("literaltab.txt");
    ofstream mcode("machinecode.txt");

    if (!ic || !symtab || !littab) {
        cout << "Error: Missing one of the input files\n";
        return 1;
    }

    unordered_map<int, int> SYMTAB;
    unordered_map<int, int> LITTAB;

    int idx, addr;
    string name;

    // symbtab: index name address
    while (symtab >> idx >> name >> addr)
        SYMTAB[idx] = addr;

    // littab: index literal address
    while (littab >> idx >> name >> addr)
        LITTAB[idx] = addr;

    string line;
    while (getline(ic, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int LC;
        if (!(ss >> LC)) continue;

        vector<string> parts;
        string tok;
        while (ss >> tok) parts.push_back(tok);
        if (parts.empty()) continue;

        string op = parts[0];

        // AD (Assembler Directive) -> no machine code
        if (op.find("(AD") != string::npos) {
            mcode << setw(3) << LC << " : " << "---- (AD)\n";
            continue;
        }

        // DL (Declarative) -> DC/DS
        if (op.find("(DL") != string::npos) {
            // extract the DL code, e.g. (DL,01)
            size_t comma = op.find(',');
            size_t close = op.find(')');
            string dlcode = (comma != string::npos && close != string::npos && close > comma) ? op.substr(comma+1, close - comma - 1) : "";
            if (dlcode == "01") {
                // DC -> next token expected (C,val)
                // find token containing (C,
                int value = 0;
                for (size_t i = 1; i < parts.size(); ++i) {
                    auto &p = parts[i];
                    if (p.find("(C,") != string::npos) {
                        // extract between "(C," and ")"
                        size_t s = p.find("(C,");
                        size_t e = p.find(')', s);
                        string num = p.substr(s + 3, e - (s + 3));
                        value = stoi(num);
                        break;
                    }
                }
                // format: opcode 00 reg address (we'll put 00 00 value)
                // ensure value printed at least 1 digit; use width 3 like original but avoid permanent setfill
                ostringstream out;
                out << setw(3) << setfill('0') << value;
                string valstr = out.str();
                mcode << setw(3) << LC << " : 00  00  " << valstr << "\n";
                cout << setfill(' '); // reset fill
            } else if (dlcode == "02") {
                // DS -> reserve N words (we'll print a comment)
                int size = 0;
                for (size_t i = 1; i < parts.size(); ++i) {
                    auto &p = parts[i];
                    if (p.find("(C,") != string::npos) {
                        size_t s = p.find("(C,");
                        size_t e = p.find(')', s);
                        string num = p.substr(s + 3, e - (s + 3));
                        size = stoi(num);
                        break;
                    }
                }
                mcode << setw(3) << LC << " : " << "---- (DS)  reserve " << size << " words\n";
            } else {
                mcode << setw(3) << LC << " : " << "---- (DL unknown)\n";
            }
            continue;
        }

        // IS (Imperative Statement)
        if (op.find("(IS") != string::npos) {
            // extract opcode number from (IS,xx)
            int opcode = 0;
            {
                size_t comma = op.find(',');
                size_t close = op.find(')');
                if (comma != string::npos && close != string::npos && close > comma) {
                    string code = op.substr(comma+1, close - comma - 1);
                    opcode = stoi(code);
                }
            }

            int regCode = 0;
            int address = 0;
            bool address_set = false;

            // parse remaining tokens for (R,x), (S,i), (L,i), (C,val)
            for (size_t i = 1; i < parts.size(); ++i) {
                string p = parts[i];
                // strip commas if any
                if (!p.empty() && p.back() == ',') p.pop_back();

                // Register: (R,x)
                if (p.rfind("(R,", 0) == 0) {
                    size_t s = p.find("(R,");
                    size_t e = p.find(')', s);
                    if (e != string::npos) {
                        string rnum = p.substr(s + 3, e - (s + 3));
                        regCode = stoi(rnum);
                    }
                }
                // Symbol: (S,i)
                else if (p.rfind("(S,", 0) == 0) {
                    size_t s = p.find("(S,");
                    size_t e = p.find(')', s);
                    if (e != string::npos) {
                        string idxs = p.substr(s + 3, e - (s + 3));
                        int symIndex = stoi(idxs);
                        if (SYMTAB.find(symIndex) != SYMTAB.end()) {
                            address = SYMTAB[symIndex];
                            address_set = true;
                        } else {
                            cerr << "Warning: symbol index " << symIndex << " not found in SYMTAB. Using 0.\n";
                            address = 0;
                            address_set = true;
                        }
                    }
                }
                // Literal: (L,i)
                else if (p.rfind("(L,", 0) == 0) {
                    size_t s = p.find("(L,");
                    size_t e = p.find(')', s);
                    if (e != string::npos) {
                        string idxs = p.substr(s + 3, e - (s + 3));
                        int litIndex = stoi(idxs);
                        if (LITTAB.find(litIndex) != LITTAB.end()) {
                            address = LITTAB[litIndex];
                            address_set = true;
                        } else {
                            cerr << "Warning: literal index " << litIndex << " not found in LITTAB. Using 0.\n";
                            address = 0;
                            address_set = true;
                        }
                    }
                }
                // Constant: (C,val)
                else if (p.find("(C,") != string::npos) {
                    size_t s = p.find("(C,");
                    size_t e = p.find(')', s);
                    if (e != string::npos) {
                        string val = p.substr(s + 3, e - (s + 3));
                        address = stoi(val);
                        address_set = true;
                    }
                }
            }

            // final output: LC : opcode reg address
            // format with width; reset setfill after use
            ostringstream out;
            out << setw(2) << setfill('0') << opcode;
            string opc = out.str();
            cout << setfill(' ');

            ostringstream out2;
            out2 << setw(2) << setfill('0') << regCode;
            string rstr = out2.str();
            cout << setfill(' ');

            ostringstream out3;
            out3 << setw(3) << setfill('0') << (address_set ? address : 0);
            string astr = out3.str();
            cout << setfill(' ');

            mcode << setw(3) << LC << " : " << opc << "  " << rstr << "  " << astr << "\n";
        }
    }

    cout << "Machine code generated in machinecode.txt\n";
    return 0;
}
