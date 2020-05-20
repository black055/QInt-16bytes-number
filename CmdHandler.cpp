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
    if (tokens[2] == ">>")
    {
        int base = stoi(tokens[0]);
        QInt a(tokens[1], base);
        int b = stoi(tokens[3]);
        return (a >> b).toString(base);
    }
    if (tokens[2] == "<<")
    {
        int base = stoi(tokens[0]);
        QInt a(tokens[1], base);
        int b = stoi(tokens[3]);
        return (a << b).toString(base);
    }
    if (tokens.size() == 4)
    {
        int base = stoi(tokens[0]);
        QInt a(tokens[1], base);
        QInt b(tokens[3], base);
        if (tokens[2] == "+")
            return (a + b).toString(base);
        if (tokens[2] == "-")
            return (a - b).toString(base);
        if (tokens[2] == "*")
            return (a * b).toString(base);
        if (tokens[2] == "/")
            return (a / b).toString(base);
        if (tokens[2] == "&")
            return (a & b).toString(base);
        if (tokens[2] == "|")
            return (a | b).toString(base);
        if (tokens[2] == "^")
            return (a ^ b).toString(base);
    }
    if (tokens.size() == 3)
    {
        if (isdigit(tokens[1][0]))
        {
            int base1 = stoi(tokens[0]);
            int base2 = stoi(tokens[1]);
            QInt a(tokens[2], base1);
            return a.toString(base2);
        }
        if (tokens[1] == "ror")
        {
            int base = stoi(tokens[0]);
            QInt a(tokens[2], base);
            QInt res = a.ror();
            return res.toString(base);
        }
        if (tokens[1] == "rol")
        {
            int base = stoi(tokens[0]);
            QInt a(tokens[2], base);
            QInt res = a.rol();
            return res.toString(base);
        }
        if (tokens[1] == "~")
        {
            int base = stoi(tokens[0]);
            QInt a(tokens[2], base);
            QInt res = ~a;
            return res.toString(base);
        }
    }
    return "FAIL";
}