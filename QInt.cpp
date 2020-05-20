﻿#include "QInt.h"

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

std::string div2(std::string number) {

	std::string result;
	int temp = 0, index = 0;
	while (temp < 2 && index < number.size()) {
		temp = temp * 10 + number[index++] - '0';
	}

	while (index <= number.size()) {
		result += (temp / 2) + '0';

		temp = (temp % 2) * 10 + (number[index++] - '0');
	}
	return result;
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
				// gan gia tri 1 tai vi tri i
				arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | arrBit[getArrayIndex(i)];
			}
		}
	}
	else if (base == 16) {
		for (int i = str.size() - 1; i >= 0; i--) {
			int temp = -1, bitIndex = 4 * (str.size() - 1 - i);
			if (str[i] >= '0' && str[i] <= '9') {
				temp = str[i] - '0';
			}
			else if (str[i] >= 'A' && str[i] <= 'F') {
				temp = str[i] + 10 - 'A';
			}
			else if (str[i] >= 'a' && str[i] <= 'f') {
				temp = str[i] + 10 - 'a';
			}
			if (temp != -1) {
				arrBit[getArrayIndex(bitIndex)] = (temp << getBitIndex(bitIndex)) | arrBit[getArrayIndex(bitIndex)];
			}
		}
	}
	else if (base == 10) {
		bool isNegative = false;
		int index = 0;
		if (str[0] == '-') {
			isNegative = true;
			str.erase(str.begin());
		}

		while (str != "0" && index < 128) {
			if ((str[str.size() - 1] - '0') % 2 == 1 ) {
				// gan gia tri 1 tai vi tri index
				arrBit[getArrayIndex(index)] = (1 << getBitIndex(index)) | arrBit[getArrayIndex(index)];
			}
			index++;
			str = div2(str);
		}

		if (isNegative == true) {
			*this = ~*this;
			*this = *this + 1;
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

		while (result.size() > 1 && result[0] == '0') {
			result.erase(result.begin());
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

		while (result.size() > 1 && result[0] == '0') {
			result.erase(result.begin());
		}
	}
	if (base == 10) {
		QInt temp(*this);
		bool isNegative = false;
		result.append(128, '0');

		if (temp.isPositive() == false) {
			isNegative = true;
			temp = temp - QInt(0, 0, 0, 1);
			temp = ~temp;	
		}
		for (int i = 0; i < 128; i++) {
			// luu gia tri bit dau
			int firstBit = temp.arrBit[0] < 0;
			temp = temp << 1;

			for (int index = result.size() - 1; index >= 0; index--) {
				result[index] += result[index] - '0' + firstBit;

				firstBit = (result[index] > '9');

				if (firstBit) {
					result[index] -= 10;
				}
			}
		}

		while (result.size() > 1 && result[0] == '0') {
			result.erase(result.begin());
		}

		if (isNegative == true) {
			result = "-" + result;
		}
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
		if (arrBit[i] < 0 && a.arrBit[i] >= 0 && temp >= 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0)
			isMissing = 1;
		if (arrBit[i] >= 0 && a.arrBit[i] < 0 && temp >= 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}
	return result;
}

QInt QInt::operator+(int a)
{
	return *this + QInt(0, 0, 0, a);
}

QInt QInt::operator-(QInt a)
{
	QInt result;
	int isMissing = 0;
	for (int i = 3; i >= 0; i--) {
		int temp = arrBit[i] - a.arrBit[i];
		temp -= isMissing;
		isMissing = 0;
		if (arrBit[i] >= 0 && a.arrBit[i] >= 0 && temp < 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0 && temp > 0)
			isMissing = 1;
		if (arrBit[i] >= 0 && a.arrBit[i] < 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}
	return result;
}

QInt QInt::operator-(int a)
{
	return *this - QInt(0, 0, 0, a);
}

QInt QInt::operator*(QInt a)
{
	QInt result;
	if (this->isZero() || a.isZero())
		return result;
	bool isNegative = false;
	QInt replace = *this;
	if (a.arrBit[0] < 0 && this->arrBit[0] < 0) {
		isNegative = false;
		QInt zero;
		a = zero - a;
		replace = zero - *this;
	}
	else if (a.arrBit[0] < 0 || this->arrBit[0] < 0) {
		isNegative = true;
		QInt zero;
		if (a.arrBit[0] < 0)
			a = zero - a;
		else
			replace = zero - *this;
	}

	QInt temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (replace.getBitAt(i * 32 + j) == 1)
			{
				temp = a << (i * 32 + j);
				result = result + temp;
			}
		}
	}
	
	if (isNegative == true)
	{
		QInt zero;
		result = zero - result;
	}
	return result;
}

QInt QInt::operator/(QInt a)
{
	// Một trong 2 số bằng 0
	if (a.isZero())
		return QInt(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
	if (this->isZero())
		return QInt(0, 0, 0, 0);

	QInt result;
	QInt replace = *this;

	// Kiểm tra số âm
	bool isNegative = false;
	if (a.arrBit[0] < 0 && this->arrBit[0] < 0) {
		isNegative = false;
		QInt zero;
		a = zero - a;
		replace = zero - replace;
	}
	else if (a.arrBit[0] < 0 || this->arrBit[0] < 0) {
		isNegative = true;
		QInt zero;
		if (a.arrBit[0] < 0)
			a = zero - a;
		else
			replace = zero - replace;
	}

	// Số bị chia và số chia bằng nhau
	if ((replace - a).isZero())
	{
		QInt temp("1", 10);
		result = temp;
	}
	// Số bị chia nhỏ hơn số chia
	else if ((replace - a).isPositive() == false)
		return QInt(0, 0, 0, 0);
	else
	{
		while (replace.isZero() == false)
		{
			QInt sub = replace - a;
			if (sub.isPositive() == false)
				break;
			QInt quotient(0, 0, 0, 1);
			for (int i = 0; i < 128; i++)
			{
				std::string check = replace.toString(10);
				std::string number_a = a.toString(10);
				QInt temp = (quotient * a);
				temp = temp - replace;
				if (temp.isPositive() && temp.isZero() == false)
				{
					quotient = quotient >> 1;
					result = result + quotient;
					replace = replace - a * quotient;
					check = replace.toString(10);
					break;
				}
				else if (temp.isPositive())
				{
					result = result + quotient;
					replace = replace - a * quotient;
					break;
				}
				quotient = quotient << 1;
				check = replace.toString(10);
			}
		}
	}

	if (isNegative == true)
	{
		QInt zero;
		result = zero - result;
	}
	return result;
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

QInt QInt::rol()
{
	QInt result;
	QInt temp;
	int bit = getBitAt(127);
	temp = *this << 1;
	temp.setBitAt(0, bit);
	result = temp;
	return result;
}

QInt QInt::ror()
{
	QInt result;
	QInt temp;
	int bit = getBitAt(0);
	temp = *this >> 1;
	temp.setBitAt(127, bit);
	result = temp;
	return result;
}

bool QInt::isPositive()
{
	if (arrBit[0] < 0)
		return false;
	else return true;
}

bool QInt::isZero()
{
	for (int index = 0; index < 4; ++index) {
		if (this->arrBit[index] != 0)
			return false;
	}

	return true;
}

int QInt::getBitAt(int index)
{
	int result;
	int position_byte = (127 - index) / 32;
	int position_bit = index % 32;
	result = (this->arrBit[position_byte] >> position_bit) & 1;
	return result;
}

void QInt::setBitAt(int index, int value)
{
	int position_byte = (127 - index) / 32;
	int position_bit = index % 32;
	if (value == 1)
		this->arrBit[position_byte] = (1 << position_bit) | this->arrBit[position_byte];
	else if (value == 0)
		this->arrBit[position_byte] = ~(1 << position_bit) & this->arrBit[position_byte];
}
