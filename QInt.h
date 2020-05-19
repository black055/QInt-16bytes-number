#pragma once
#include <iostream>
#include <string>

class QInt {
private:
	int arrBit[4];
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

	bool isPositive();
	bool isZero();
	int getBitAt(int index);
};