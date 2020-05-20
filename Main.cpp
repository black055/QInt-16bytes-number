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
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	ioHandle(in, out);
	in.close();
	out.close();
	return 0;
}

void ioHandle(istream& in, ostream& out)
{
	string cmd;
	while (getline(in, cmd))
	{
		out << CmdHandler().handle(cmd) << endl;
	}
}
