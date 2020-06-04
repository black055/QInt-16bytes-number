#include "CmdHandler.h"
#include <sstream>

using namespace std;

// tách chuỗi str thành các tokens, trả về vector các chuỗi tokens
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

// Nhận diện loại phép tính từ chuỗi cmd, tính toán và trả về kết quả dưới dạng string
std::string CmdHandler::handle(std::string cmd)
{
    vector<string> tokens = tokenize(cmd); // tách các thành phần trong cmd ra thành mảng các chuỗi
    
    //CÁC LOẠI PHÉP TÍNH:
    //Phép tính 2 ngôi: + - * / & | ^ >> <<
    //    format: <base> <operand1> <operator> <operand2>
    //Phép tính 1 ngôi: ror rol ~
    //    format: <base> <operator> <operand>
    //Phép chuyển đổi hệ cơ số:
    //    format: <from base> <to base> <operand>
        
    if (tokens[2] == ">>") // Phép dịch phải
    {
        int base = stoi(tokens[0]);
        QInt a(tokens[1], base);
        int b = stoi(tokens[3]);
        return (a >> b).toString(base);
    }
    if (tokens[2] == "<<") // Phép dịch trái
    {
        int base = stoi(tokens[0]);
        QInt a(tokens[1], base);
        int b = stoi(tokens[3]);
        return (a << b).toString(base);
    }
    if (tokens.size() == 4) // Phép toán 2 ngôi khác
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
        // Phép đổi cơ số
        if (isdigit(tokens[1][0]))
        {
            int base1 = stoi(tokens[0]);
            int base2 = stoi(tokens[1]);
            QInt a(tokens[2], base1);
            return a.toString(base2);
        }

        // các phép toán 1 ngôi
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