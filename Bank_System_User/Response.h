#pragma once
#include <string>
#include <vector>
//#include <windows.h>
#include "user_info.h"
#include "basic_functions.h"
#include <iostream>

using namespace std;


class Response {
public:
    virtual bool need_execute() const { return true; };

    virtual ~Response() = default;
    virtual void get_response(handleInfo& handle) = 0;
    virtual std::string name() const = 0;
};