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


// --- допоміжна функція для обрізання пробілів ---
void trim(std::string& s) {
    while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
}

// --- читання з cfg файлу ---
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

// --- запис у cfg файл ---
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