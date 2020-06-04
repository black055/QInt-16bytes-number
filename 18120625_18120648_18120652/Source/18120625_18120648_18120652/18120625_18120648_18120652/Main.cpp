#include <fstream>
#include "CmdHandler.h"
#include <string>

using namespace std;

void ioHandle(istream& in, ostream& out);

void test()
{
	ifstream in("input.txt");
	ofstream out("output.txt");
	ioHandle(in, out);
	in.close();
	out.close();
}

int main(int argc, char** argv)
{
	//test(); // test here
	//return 0;
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	ioHandle(in, out);
	in.close();
	out.close();
	return 0;
}

// Nhiệm vụ: đọc từng dòng trong input, xử lý rồi ghi kết quả ra output
void ioHandle(istream& in, ostream& out)
{
	string cmd;
	while (getline(in, cmd))
	{
		try
		{
			// Protected code block
			out << CmdHandler().handle(cmd) << endl;
		}
		catch (const char* msg)
		{
			out << 0 << endl;
		}
	}
}
