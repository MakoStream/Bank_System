#include "basic_functions.h"




vector<string> split(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        tokens.push_back(word);
    };
    return tokens;
};