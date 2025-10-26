#include "Command.h"


// Function: throw_response
// Description: Copies a message into the first slot of sessionData.msg and writes the sessionData 
//              structure to the pipe associated with the handle.
// Requirements: <string>, <cstring>, Windows.h (for WriteFile)
// Required for: class Command (and all inherited class)
void throw_response(handleInfo& handle, const std::string& msg) {
    strncpy(handle.sessionData.msg[0], msg.c_str(), sizeof(handle.sessionData.msg[0]) - 1);
    handle.sessionData.msg[0][sizeof(handle.sessionData.msg[0]) - 1] = '\0';
    WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
    return;
};