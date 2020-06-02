#pragma once
#include <iostream>
#include <string>

class QInt {
private:
	int arrBit[4];

	static int getArrayIndex(int i);
	static int getBitIndex(int i);
	static std::string div2(std::string number);

public:
	QInt();
	QInt(int a, int b, int c, int d);
	QInt(std::string str, int base);

	std::string toString(int base);

	void operator = (QInt a);

	QInt operator + (QInt a);
	QInt operator + (int a);
	QInt operator - (QInt a);
	QInt operator - (int a);
	QInt operator * (QInt a);
	QInt operator / (QInt a);

	QInt operator & (QInt a);
	QInt operator | (QInt a);
	QInt operator ^ (QInt a);
	QInt operator ~ ();

	QInt operator << (int a);
	QInt operator >> (int a);

	bool operator == (QInt a);
	bool operator != (QInt a);

	QInt rol();
	QInt ror();

	bool isPositive();
	bool isZero();
	int getBitAt(int index);
	void setBitAt(int index, int value);

	static const QInt MINQINT;
};