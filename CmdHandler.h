#pragma once
#include <string>
#include <vector>

class CmdHandler
{
private:
	std::vector<std::string> tokenize(std::string str);

public:
	std::string handle(std::string cmd);
};
