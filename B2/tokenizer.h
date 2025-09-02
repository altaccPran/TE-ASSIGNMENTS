#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class Tokenizer {
public:
    std::vector<std::string> getTokens(const std::string &line);
private:
    bool isOpcode(const std::string &word);
};
