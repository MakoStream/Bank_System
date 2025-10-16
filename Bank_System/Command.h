#pragma once
#include "mainProcess.h"
#include <string>
#include <vector>
#include <windows.h>
#include "basic_functions.h"
#include "User.h"
#include "Account.h"

using namespace std;




// Базовий клас для будь-якої команди
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(handleInfo& handle) = 0; // чисто віртуальний метод
    virtual std::string name() const = 0; // ім'я команди
};