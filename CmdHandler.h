#pragma once
#include <string>
#include <vector>
#include "QInt.h"

// class có nhiệm vụ xử lý phép tính từ chuỗi
class CmdHandler
{
private:
	std::vector<std::string> tokenize(std::string str);

public:
	std::string handle(std::string cmd);
};
