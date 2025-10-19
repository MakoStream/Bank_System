#include "Command.h"

void throw_response(handleInfo& handle, const std::string& msg) {
    strncpy(handle.sessionData.msg[0], msg.c_str(), sizeof(handle.sessionData.msg[0]) - 1);
    handle.sessionData.msg[0][sizeof(handle.sessionData.msg[0]) - 1] = '\0';
    WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
    return;
};