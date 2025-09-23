#pragma once
#include "../mainProcess.h"
#include "../Logger.h"
#include "../DB_op.h"
#include "../Command.h"
#include <iostream>

class CreateDBCommand : public Command {
    Logger* logger;  // �������� �� ���������� ������
public:
    void execute(const std::vector<std::string>& args, Session& user) override {
        DB_create();
        //std::cout << "���� ����� ��������!\n" 
    }

    std::string name() const override {
        return "create_db";
    }
};