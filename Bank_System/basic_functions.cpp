#include "basic_functions.h"



// Function: isStringDigit
// Description: Checks whether the given string represents a valid numeric value (integer or decimal).
// Requirements: <string>, <cctype>
// Required for: class NewAccountCommand, class UserListCommand, class AccountListCommand, class UserInfoCommand,
//               class RegisterUserCommand
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

// Function: split
// Description: Splits a string into words separated by whitespace and returns them as a vector
// Requirements: <string>, <sstream>, <vector>
// Required for: main(), Command::execute()
vector<string> split(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        tokens.push_back(word);
    };
    return tokens;
};

// Function: trim
// Description: Removes spaces at the edges of text
// Requirements: <string>, <sstream>
// Required for: readConfig()
void trim(std::string& s) {
    while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
}

// Function: readConfig
// Description: Reads configuration attributes from a *.ini file and stores them as key-value pairs.
// Requirements: <iostream>, <fstream>, <string>, <map>, and a helper function trim(std::string&)
// Required for: class mainProcess()
std::map<std::string, std::string> readConfig(const std::string& filename) {
	cout << "Reading config from " << filename << std::endl;
    std::ifstream file(filename);
    std::map<std::string, std::string> config;

    if (!file.is_open()) {
        cout << "Cannot open " << filename << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // коментарі

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        trim(key);
        trim(value);

        config[key] = value;
    }

    file.close();
    return config;
}

// Function: writeConfig
// Description: Writes configuration key-value pairs to a specified *.ini file, overwriting its content.
// Requirements: <iostream>, <fstream>, <string>, <map>
// Required for: class mainProcess()
void writeConfig(const std::string& filename, const std::map<std::string, std::string>& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot write in " << filename << std::endl;
        return;
    }

    for (const auto& [key, value] : config) {
        file << key << "=" << value << "\n";
    }

    file.close();
    return;
}