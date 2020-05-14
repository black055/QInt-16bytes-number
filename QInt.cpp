#include "QInt.h"

int getArrayIndex(int i) {
	if (i > 127 || i < 0)
		return -1;
	return ((127 - i) / 32);
}

int getBitIndex(int i) {
	if (i > 127 || i < 0)
		return -1;
	return i % 32;
}

QInt::QInt() {
	for (int i = 0; i < 4; i++) {
		arrBit[i] = 0;
	}
}

QInt::QInt(int a, int b, int c, int d)
{
	arrBit[0] = a;
	arrBit[1] = b;
	arrBit[2] = c;
	arrBit[3] = d;
}

QInt::QInt(std::string arrInt, int base)
{

}

std::string QInt::toString(int base)
{
	return std::string();
}

void QInt::operator=(QInt a)
{
	for (int i = 0; i < 4; i++) {
		this->arrBit[i] = a.arrBit[i];
	}
}

QInt QInt::operator+(QInt a)
{
	QInt result;
	int isMissing = 0;
	for (int i = 3; i >= 0; i--) {
		int temp = arrBit[i] + a.arrBit[i];
		temp += isMissing;
		isMissing = 0;
		if (arrBit[i] < 0 && a.arrBit[i] > 0 && temp > 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0)
			isMissing = 1;
		if (arrBit[i] > 0 && a.arrBit[i] < 0 && temp > 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}
	return result;
}

QInt QInt::operator-(QInt a)
{
	QInt result;
	int isMissing = 0;
	for (int i = 3; i >= 0; i--) {
		int temp = arrBit[i] - a.arrBit[i];
		temp -= isMissing;
		isMissing = 0;
		if (arrBit[i] > 0 && a.arrBit[i] > 0 && temp < 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0 && temp < 0)
			isMissing = 1;
		if (arrBit[i] > 0 && a.arrBit[i] < 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}
	return result;
}

QInt QInt::operator*(QInt a)
{
	return QInt();
}

QInt QInt::operator/(QInt a)
{
	return QInt();
}

QInt QInt::operator&(QInt a)
{
	QInt result;
	for (int i = 0; i < 4; i++) {
		result.arrBit[i] = arrBit[i] & a.arrBit[i];
	}
	return result;
}

QInt QInt::operator|(QInt a)
{
	QInt result;
	for (int i = 0; i < 4; i++) {
		result.arrBit[i] = arrBit[i] | a.arrBit[i];
	}
	return result;
}

QInt QInt::operator^(QInt a)
{
	QInt result;
	for (int i = 0; i < 4; i++) {
		result.arrBit[i] = arrBit[i] ^ a.arrBit[i];
	}
	return result;
}

QInt QInt::operator~()
{
	return QInt(~arrBit[0], ~arrBit[1], ~arrBit[2], ~arrBit[3]);
}

QInt QInt::operator<<(int a)
{
	return QInt();
}

QInt QInt::operator>>(int a)
{
	QInt result;

	bool isNegative = (arrBit[0] < 0) ? true: false;

	for (int i = 0; i <= 127 - a; i++) {
		if ((arrBit[getArrayIndex(i + a)] >> getBitIndex(i + a) & 1) == 0) {
			// gan gia tri 0 tai vi tri i neu vi tri i + a la bit 0
			result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
		}
		else {
			// gan gia tri 1 tai vi tri i neu vi tri i + a la bit i
			result.arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | result.arrBit[getArrayIndex(i)];
		}
	}

	for (int i = 127; i >= 0 && i > 127 - a; i++) {
		if (isNegative == true) {
			result.arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | result.arrBit[getArrayIndex(i)];
		}
		else {
			result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
		}
	}
	return result;
}
