#include "basic_functions.h"




bool isStringDigit(const std::string& s) {
    if (s.empty()) return false;
    int dotCount = 0;
    for (char c : s) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        }
        else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Function: isPhone
// Description: Validates if a string is a 10-digit phone number
// Requirements: <string>, <cctype>
// Required for: commands::execute("reg_user")
bool isPhone(const std::string& s) {
    if (s.empty()) return false;
    if (s.size() != 10) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

vector<string> split(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        tokens.push_back(word);
    };
    return tokens;
};