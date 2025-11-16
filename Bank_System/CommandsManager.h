/**
@file CommandsManager.h
@brief Manages all command objects and executes them based on client input.
@details This header defines the CommandsManager class, which stores all available
commands as objects and provides an interface to execute them according to
the command string received from clients.
@note Required headers: Command.h, basic_functions.h, <vector>, <memory>, <string>.
*/

#pragma once
#include "Command.h"
#include <vector>
#include <memory>
#include <string>
#include "basic_functions.h"

class CommandsManager {
private:
    std::vector<std::unique_ptr<Command>> commands;
public:
    /**
    @brief Manages and executes all available commands.
    @details Stores all command objects in a vector and matches incoming command strings
    from client sessions to the appropriate Command object for execution.
    */
    CommandsManager();

    /**
    @brief Constructor that registers all available commands.
    @details Initializes the vector of commands, including debug and test commands.
    @note Required headers: <vector>, <memory>, <string>, headers of all command classes.
    */
    void execute(handleInfo& handle);
};