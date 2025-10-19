#pragma once
#include "Response.h"
#include <vector>
#include <memory>
#include <string>
#include "basic_functions.h"

class ResponseManager {
private:
    std::vector<std::unique_ptr<Response>> responses;
public:
    ResponseManager();

    void get_response(handleInfo& handle);
};

extern ResponseManager cmd;