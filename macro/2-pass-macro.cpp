#include <bits/stdc++.h>
using namespace std;

// ---------- Utilities ----------
static inline string ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char c){return !isspace(c);})); return s;
}
static inline string rtrim(string s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char c){return !isspace(c);}).base(), s.end()); return s;
}
static inline string trim(string s) { return rtrim(ltrim(std::move(s))); }

static vector<string> splitCommaArgs(const string &s) {
    vector<string> out; string token; 
    for (size_t i=0;i<s.size();++i) {
        char c=s[i];
        if (c==',') { out.push_back(trim(token)); token.clear(); }
        else token.push_back(c);
    }
    if (!token.empty()) out.push_back(trim(token));
    // Allow empty actuals (e.g., trailing comma)
    return out;
}

static vector<string> splitWords(const string &s) {
    vector<string> out; string w; stringstream ss(s);
    while (ss >> w) out.push_back(w);
    return out;
}

static string replaceAll(const string &src, const unordered_map<string,string> &sub) {
    // Since formals start with '&', naive replace is acceptable
    string res = src;
    for (const auto &kv : sub) {
        const string &pat = kv.first, &rep = kv.second;
        size_t pos = 0;
        while ((pos = res.find(pat, pos)) != string::npos) {
            res.replace(pos, pat.size(), rep);
            pos += rep.size();
        }
    }
    return res;
}

// ---------- Data structures ----------
struct MNTEntry {
    string name;
    int mdtIndex;                 // start index in MDT for this macro's body
    vector<string> formals;       // e.g., {"&A","&B"}
};

struct MDTEntry {
    int index;
    string line;                  // one body line; "MEND" marks end
};

// ---------- Global tables ----------
vector<MNTEntry> MNT;
vector<MDTEntry> MDT;
vector<string>   IC;              // intermediate code (no macro definitions)

// ---------- Pass-I: build MNT, MDT, IC ----------
void pass1_build_tables(const string &inputAsmPath) {
    ifstream in(inputAsmPath);
    if (!in) { cerr << "Error: cannot open " << inputAsmPath << "\n"; exit(1); }

    bool inMacro = false;
    string line;
    int mdtIdxCounter = 0;
    bool expectingHeader = false;  // after "MACRO"
    MNTEntry current;

    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Recognize MACRO and MEND in a whitespace-tolerant way
        string upper = line; for (auto &c: upper) c = toupper(c);

        if (!inMacro && upper == "MACRO") {
            inMacro = true;
            expectingHeader = true;
            continue;
        }

        if (inMacro && expectingHeader) {
            // header format: NAME [params...], typical: NAME &A,&B  (params may be absent)
            // split first word as macro name, remainder as parameter list (comma-separated)
            auto spacePos = line.find_first_of(" \t");
            string name = (spacePos == string::npos) ? line : line.substr(0, spacePos);
            string rest = (spacePos == string::npos) ? ""   : trim(line.substr(spacePos+1));

            vector<string> formals;
            if (!rest.empty()) formals = splitCommaArgs(rest);

            // normalize formals to ensure they start with '&'
            for (auto &f : formals) {
                f = trim(f);
                if (!f.empty() && f[0] != '&') f = "&" + f;
            }

            current = { name, mdtIdxCounter, formals };
            MNT.push_back(current);
            expectingHeader = false; // next lines belong to body
            continue;
        }

        if (inMacro) {
            if (upper == "MEND") {
                MDT.push_back({mdtIdxCounter++, "MEND"});
                inMacro = false;
                continue;
            } else {
                MDT.push_back({mdtIdxCounter++, line});
                continue;
            }
        }

        // Outside macro definition -> goes to IC
        IC.push_back(line);
    }

    // Also write Pass-I artifacts to files (for inspection / grading)
    {
        ofstream mntFile("MNT.txt");
        for (auto &e : MNT) {
            // Print formals after name (nice for reading), and MDT start
            mntFile << e.name << " " << e.mdtIndex;
            if (!e.formals.empty()) {
                mntFile << "   "; // spacer
                for (size_t i=0;i<e.formals.size();++i) {
                    if (i) mntFile << ",";
                    mntFile << e.formals[i];
                }
            }
            mntFile << "\n";
        }
    }
    {
        ofstream mdtFile("MDT.txt");
        for (auto &e : MDT) {
            mdtFile << e.index << " " << e.line << "\n";
        }
    }
    {
        ofstream icFile("IC.txt");
        for (auto &l : IC) icFile << l << "\n";
    }
}

// ---------- Helper: find macro by name ----------
const MNTEntry* findMacro(const string &name) {
    for (const auto &e : MNT) if (e.name == name) return &e;
    return nullptr;
}

// ---------- Pass-II: expand macros in IC using MNT/MDT ----------
void pass2_expand_to_output(const string &outputPath) {
    ofstream out(outputPath);
    if (!out) { cerr << "Error: cannot open " << outputPath << " for writing\n"; exit(1); }

    for (const string &rawLine : IC) {
        string line = trim(rawLine);
        if (line.empty()) { out << "\n"; continue; }

        // Tokenize first word (potential macro name), then parse args (if any)
        size_t sp = line.find_first_of(" \t");
        string first = (sp == string::npos) ? line : line.substr(0, sp);
        string rest  = (sp == string::npos) ? ""   : trim(line.substr(sp+1));

        const MNTEntry* macro = findMacro(first);
        if (!macro) {
            // Not a macro call -> copy as-is
            out << rawLine << "\n";
            continue;
        }

        // Parse actuals: allow either "A,B" or "A, B" etc.
        vector<string> actuals;
        if (!rest.empty()) actuals = splitCommaArgs(rest);

        // Build ALA formal->actual
        unordered_map<string,string> ALA;
        for (size_t i = 0; i < macro->formals.size(); ++i) {
            string formal = macro->formals[i];
            string actual = (i < actuals.size()) ? actuals[i] : string("");
            ALA[formal] = actual;
        }

        // Walk MDT from mdtIndex until MEND
        for (int i = macro->mdtIndex; i < (int)MDT.size(); ++i) {
            const string &mdtLine = MDT[i].line;
            if (mdtLine == "MEND") break;

            // Replace all &params with actuals
            string expanded = replaceAll(mdtLine, ALA);
            out << expanded << "\n";
        }
    }
    out.close();
}

// ---------- Main: run Pass-I then Pass-II ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string inputAsm = "input.asm";  // change path if needed
    const string finalOut = "output.txt";

    pass1_build_tables(inputAsm);
    pass2_expand_to_output(finalOut);

    cout << "Pass-I complete: wrote MNT.txt, MDT.txt, IC.txt\n";
    cout << "Pass-II complete: wrote " << finalOut << "\n";
    return 0;
}
