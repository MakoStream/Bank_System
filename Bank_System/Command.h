/**
@file Command.h
@brief Defines the base Command class and utility functions for sending responses to clients.
@details This header provides the abstract Command class, which serves as the base
for all specific command implementations. It also declares the throw_response function
for sending messages back to clients via a named pipe.
@note Required headers: mainProcess.h, basic_functions.h, User.h, Account.h, <string>, <vector>, <windows.h>.
*/

#pragma once
#include "mainProcess.h"
#include <string>
#include <vector>
#include <windows.h>
#include "basic_functions.h"
#include "User.h"
#include "Account.h"

using namespace std;




/**
@brief Abstract base class for all commands.
@details Defines the interface that all command classes must implement.
Each derived command class must provide an execute method and a method to return its name.
*/
class Command {
public:
    /**
     * @brief Virtual destructor.
     * @details Ensures that derived command classes can be properly
     *          destroyed through a base class pointer.
     */
    virtual ~Command() = default;

    /**
     * @brief Executes the command.
     * @details
     * This pure virtual method must be implemented by every derived command class.
     * It contains the logic for processing the command, including validation,
     * state modification, and interaction with the handleInfo object.
     *
     * @param handle Reference to handleInfo, which contains session data,
     *               input commands, and other necessary context for execution.
     */
    virtual void execute(handleInfo& handle) = 0; // only virtual method

    /**
     * @brief Returns the name of the command.
     * @details
     * This pure virtual method must be implemented by each derived command class.
     * It provides a unique identifier for the command, typically the text
     * used to invoke it in the command-line or network interface.
     *
     * @return std::string The command name as a string.
     */
    virtual std::string name() const = 0; // command name
};

/**
@brief Sends a response message to the client.
@details Copies the given message into the first slot of sessionData.msg
and writes the updated sessionData structure to the pipe associated with the handle.
@param handle Reference to handleInfo containing the pipe and sessionData.
@param msg The message string to send to the client.
@note Required headers: <string>, <cstring>, <windows.h>.
@note Used by Command and all derived command classes to send messages to clients.
*/
void throw_response(handleInfo& handle, const std::string& msg);