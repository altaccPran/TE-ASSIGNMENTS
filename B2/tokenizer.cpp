#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class Tokenizer {
public:
    // Splits a line into exactly: LABEL, OPCODE, OPERAND1, OPERAND2
    std::vector<std::string> getTokens(const std::string &line) {
        std::vector<std::string> tokens(4, ""); // default 4 empty tokens
        std::vector<std::string> rawTokens;

        std::stringstream ss(line);
        std::string token;

        while (ss >> token) {
            // Remove trailing commas
            if (!token.empty() && token.back() == ',') {
                token.pop_back();
            }
            rawTokens.push_back(token);
        }

        // Determine if first token is label or opcode
        if (rawTokens.size() == 0) return tokens;

        if (isOpcode(rawTokens[0])) {
            // No label present
            tokens[0] = ""; // label
            tokens[1] = rawTokens[0]; // opcode
            if (rawTokens.size() > 1) tokens[2] = rawTokens[1];
            if (rawTokens.size() > 2) tokens[3] = rawTokens[2];
        } else {
            // Label present
            tokens[0] = rawTokens[0];
            if (rawTokens.size() > 1) tokens[1] = rawTokens[1];
            if (rawTokens.size() > 2) tokens[2] = rawTokens[2];
            if (rawTokens.size() > 3) tokens[3] = rawTokens[3];
        }
        return tokens;
    }

private:
    bool isOpcode(const std::string &word) {
        static const std::vector<std::string> opcodes = {
            "STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV",
            "READ","PRINT","DC","DS","START","END","ORIGIN","EQU","LTORG"
        };
        return std::find(opcodes.begin(), opcodes.end(), word) != opcodes.end();
    }
};
