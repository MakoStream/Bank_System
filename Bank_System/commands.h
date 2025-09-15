#pragma once
#include "DB_op.h"
#include <string>
#include <vector>



class commands {

public:
	commands();
	void execute(const std::vector<std::string>& args);
private:
	void createDB();
	void regUser(User user);
	void printUserList();
};
