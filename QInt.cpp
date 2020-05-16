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

QInt::QInt(std::string str, int base)
{
	for (int i = 0; i < 4; i++){
		arrBit[i] = 0;
	}
	if (base == 2) {
		for (int i = 0; i < str.size() && i < 128; i++) {
			if (str[str.size() - 1 - i] == '0') {
				// gan gia tri 0 tai vi tri i
				arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & arrBit[getArrayIndex(i)];
			}
			else {
				// gan gia tri 1 tai vi tri i neu vi tri i + a la bit i
				arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | arrBit[getArrayIndex(i)];
			}
		}
	}
	else if (base == 16) {
		for (int i = str.size() - 1; i >= 0; i--) {
			int temp, bitIndex = 4 * (str.size() - 1 - i);
			if (str[i] >= '0' && str[i] <= '9') {
				temp = str[i] - '0';
			}
			else if (str[i] >= 'A' && str[i] <= 'F') {
				temp = str[i] + 10 - 'A';
			}
			else if (str[i] >= 'a' && str[i] <= 'f') {
				temp = str[i] + 10 - 'a';
			}
			for (int i = 0; i < str.size() * 4; i = i + 4);
			arrBit[getArrayIndex(bitIndex)] = (temp << getBitIndex(bitIndex)) | arrBit[getArrayIndex(bitIndex)];
		}
	}
}

std::string QInt::toString(int base)
{
	std::string result;
	if (base == 2) {
		for (int i = 127; i >= 0; i--) {
			result += (arrBit[getArrayIndex(i)] >> (getBitIndex(i)) & 1) + '0';
		}
	}
	if (base == 16) {
		for (int arrIndex = 3; arrIndex >= 0; arrIndex--) {
			for (int i = 0; i < 8; i++) {
				char temp = (arrBit[arrIndex] >> i * 4 & 15);
				if (temp < 10) {
					temp = temp + '0';
				}
				else {
					temp = temp - 10 + 'A';
				}
				result = temp + result;
			}
		}
	}
	if (base == 10) {
		QInt temp(*this);
		bool isNegative = false;
		result.append(128, '0');
		std::string bitA; int count = 0;
		if (temp.isPositive() == false) {
			isNegative = true;
			temp = ~temp;
			temp = temp + QInt(0, 0, 0, 1);
		}
		for (int i = 0; i < 128; i++) {
			// luu gia tri bit dau
			int firstBit = temp.arrBit[0] < 0;
			temp = temp << 1;

			if (firstBit) {
				count = 1;
			}

			for (int index = result.size() - 1; index >= 0; index--) {
				result[index] += result[index] - '0' + firstBit;

				firstBit = (result[index] > '9');

				if (firstBit) {
					result[index] -= 10;
				}
			}
			if (count == 1) {
				char aa = '0' + firstBit;
				bitA = aa + bitA;
			}
		}
		if (isNegative == true) {
			result = "-" + result;
		}
	}
	while (result.size() > 1 && result[0] == '0') {
		result.erase(result.begin());
	}

	return result;
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
	if (a < 128) {
		QInt result;

		bool isNegative = (arrBit[0] < 0) ? true : false;

		for (int i = 127; i >= a; i--) {
			if ((arrBit[getArrayIndex(i - a)] >> getBitIndex(i - a) & 1) == 0) {
				// gan gia tri 0 tai vi tri i neu vi tri i - a la bit 0
				result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
			}
			else {
				// gan gia tri 1 tai vi tri i neu vi tri i - a la bit i
				result.arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | result.arrBit[getArrayIndex(i)];
			}
		}

		for (int i = 0; i < a; i++) {
			result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
		}
		return result;
	}
	return QInt(*this);
}

QInt QInt::operator>>(int a)
{
	if (a < 128) {
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

		for (int i = 127; i >= 0 && i > 127 - a; i--) {
			if (isNegative == true) {
				result.arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | result.arrBit[getArrayIndex(i)];
			}
			else {
				result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
			}
		}
		return result;
	}
	return QInt(*this);
}

bool QInt::isPositive()
{
	if (arrBit[0] < 0)
		return false;
	else return true;
}
