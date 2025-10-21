#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define byte BYTE
#include <windows.h>
//#include <boost/test/unit_test.hpp>
//#include <boost/test/unit_test_monitor.hpp>
#include "../basic_functions.h"
#include "../user_info.h"
#include "../ResponseManager.h"

void runUnitTest(handleInfo& handle);