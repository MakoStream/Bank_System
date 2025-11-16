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
    virtual ~Command() = default;
    virtual void execute(handleInfo& handle) = 0; // only virtual method
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