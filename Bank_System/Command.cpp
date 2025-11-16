#include "Command.h"


/**
@brief Sends a response message to the client.
@details Copies the given message into the first slot of sessionData.msg
and writes the updated sessionData structure to the pipe associated with the handle.
@param handle Reference to handleInfo containing the pipe and sessionData.
@param msg The message string to send to the client.
@note Required headers: <string>, <cstring>, <windows.h>.
@note Used by Command and all derived command classes to send messages to clients.
*/
void throw_response(handleInfo& handle, const std::string& msg) {
    strncpy(handle.sessionData.msg[0], msg.c_str(), sizeof(handle.sessionData.msg[0]) - 1);
    handle.sessionData.msg[0][sizeof(handle.sessionData.msg[0]) - 1] = '\0';
    WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
    return;
};