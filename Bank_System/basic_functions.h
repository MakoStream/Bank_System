#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <cctype>
#include <iostream>
#include <cctype>
#include <ctime>
#include <iomanip>

using namespace std;


bool isStringDigit(const std::string& s);
bool isPhone(const std::string& s);
vector<string> split(const string& input);
string trim(const string& str);
std::map<std::string, std::string> readConfig(const std::string& filename);
void writeConfig(const std::string& filename, const std::map<std::string, std::string>& config);
string getTimestamp();