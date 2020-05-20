#include "CmdHandler.h"
#include <sstream>

using namespace std;

std::vector<std::string> CmdHandler::tokenize(std::string str)
{
    stringstream ss(str);
    vector<string> tokens;
    string tmp;
    while (getline(ss, tmp, ' '))
    {
        tokens.push_back(tmp);
    }
    return tokens;
}

std::string CmdHandler::handle(std::string cmd)
{
    vector<string> tokens = tokenize(cmd);
    return tokens[1];
}

