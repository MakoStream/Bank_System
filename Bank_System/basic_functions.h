/**
@file basic_functions.h
@brief Helper functions for string, number, and configuration handling.
@details This header defines utility functions used throughout the bank system,
including string validation, string splitting and trimming, configuration file
reading and writing, and timestamp generation.
@note Required for classes and functions like NewAccountCommand, UserListCommand,
AccountListCommand, UserInfoCommand, RegisterUserCommand, mainProcess, and
Command::execute().
*/

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

/**
@brief Checks whether the given string represents a valid numeric value (integer or decimal).
@details Validates a string to determine if it contains only digits, optionally with one decimal point.
@param s The input string to check.
@return true if the string is a valid number, false otherwise.
@note Required headers: <string>, <cctype>.
@note Used by NewAccountCommand, UserListCommand, AccountListCommand, UserInfoCommand, RegisterUserCommand.
*/
bool isStringDigit(const std::string& s);

/**
@brief Validates if a string is a 10-digit phone number.
@details Checks that the string contains exactly 10 digits and no other characters.
@param s The input string representing the phone number.
@return true if the string is a valid 10-digit phone number, false otherwise.
@note Required headers: <string>, <cctype>.
@note Used by commands::execute("reg_user").
*/
bool isPhone(const std::string& s);

/**
@brief Splits a string into words separated by whitespace and returns them as a vector.
@details Parses the input string using whitespace as the delimiter and stores each word in a vector.
@param input The string to split.
@return A vector containing all words from the input string.
@note Required headers: <string>, <sstream>, <vector>.
@note Used in main() and Command::execute().
*/
std::vector <std::string> split(const std::string& input);

/**
@brief Removes leading and trailing whitespace from a string.
@details Trims spaces, tabs, and other whitespace characters from both ends of the input string.
@param s The string to trim (passed by reference).
@note Required headers: <string>, <sstream>.
@note Used in readConfig().
*/
void trim(std::string& s);

/**
@brief Reads configuration key-value pairs from a *.ini file.
@details Opens a specified configuration file, reads each line in the form key=value,
trims whitespace, ignores empty lines and comments (lines starting with #),
and returns a map of keys and values.
@param filename The path to the configuration file.
@return A map containing all configuration key-value pairs found in the file.
@note Required headers: <iostream>, <fstream>, <string>, <map>.
@note Uses helper function trim(std::string&).
@note Used in mainProcess().
*/
std::map<std::string, std::string> readConfig(const std::string& filename);

/**
@brief Writes configuration key-value pairs to a *.ini file.
@details Overwrites the specified configuration file with the provided key-value pairs.
@param filename The path to the configuration file.
@param config A map containing configuration keys and values to write.
@note Required headers: <iostream>, <fstream>, <string>, <map>.
@note Used in mainProcess().
*/
void writeConfig(const std::string& filename, const std::map<std::string, std::string>& config);

/**
@brief Returns the current timestamp formatted as YYYY-MM-DD_HH-MM-SS.
@details Generates a string representing the current system time for logging or file naming purposes.
@return A string containing the formatted timestamp.
@note Required headers: <chrono>, <iomanip>, <sstream>, <ctime>.
*/
std::string getTimestamp();