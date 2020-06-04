#include "QInt.h"

int QInt::getArrayIndex(int i) {
	if (i > 127 || i < 0)
		return -1;
	return ((127 - i) / 32);
}

int QInt::getBitIndex(int i) {
	if (i > 127 || i < 0)
		return -1;
	return i % 32;
}

std::string QInt::div2(std::string number) {
	// Trả về number / 2 (dưới dạng String)
	std::string result;
	int temp = 0, index = 0;
	while (temp < 2 && index < number.size()) {
		// Giá trị temp là n chữ số đầu sao cho temp > 2
		temp = temp * 10 + number[index++] - '0';
	}

	while (index <= number.size()) {
		// lần lượt chia temp cho 2 va cộng vao chuỗi result kết quả
		result += (temp / 2) + '0';
		// hạ từ chuỗi input xuống temp thêm 1 giá trị
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
	// constructor sử dụng 1 string ở hệ cơ số base để khởi tạo giá trị QInt
	for (int i = 0; i < 4; i++){
		arrBit[i] = 0;
	}
	if (base == 2) {
		for (int i = 0; i < str.size() && i < 128; i++) {
			if (str[str.size() - 1 - i] == '0') {
				// gán giá trị 0 tại vị trí i
				arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & arrBit[getArrayIndex(i)];
			}
			else {
				// gán giá trị 1 tại vị trí i
				arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | arrBit[getArrayIndex(i)];
			}
		}
	}
	else if (base == 16) {
		for (int i = str.size() - 1; i >= 0; i--) {
			// lần lượt đọc từng kí tự va gán theo nhóm 4 bit tương ứng với giá trị của kí tự đó
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
			// nếu là số âm thì tìm biểu diễn QInt của giá trị tuyệt đối của nó trước
			isNegative = true;
			str.erase(str.begin());
		}

		while (str != "0" && index < 128) {
			if ((str[str.size() - 1] - '0') % 2 == 1 ) {
				// gán giá trị 1 tại vị trí index
				arrBit[getArrayIndex(index)] = (1 << getBitIndex(index)) | arrBit[getArrayIndex(index)];
			}
			index++;

			// str = str / 2;
			str = div2(str);
		}

		if (isNegative == true) {
			// nếu là số âm thì gán vào this giá trị bù 2
			// trường hợp đặc biệt MINQINT ko cần đổi vì sẽ bị tràn số
			if (*this == MINQINT) {}
			else
			{
				*this = ~*this;
				*this = *this + 1;
			}
		}
	}
}

std::string QInt::toString(int base)
{
	// hàm trả về 1 string là biểu diễn của QInt ở hệ cơ số base
	std::string result;
	if (base == 2) {
		// lần lượt đọc và gán từng bit của QInt
		for (int i = 127; i >= 0; i--) {
			result += (arrBit[getArrayIndex(i)] >> (getBitIndex(i)) & 1) + '0';
		}

		while (result.size() > 1 && result[0] == '0') {
			result.erase(result.begin());
		}
	}
	if (base == 16) {
		// nếu là hệ cơ số 16 thì đọc theo nhóm 4 bit và ghi lại kí tự tương ứng
		for (int arrIndex = 3; arrIndex >= 0; arrIndex--) {
			for (int i = 0; i < 8; i++) {
				// lưu lại giá trị của nhóm 4 bit và chuyển thành kí tự
				char temp = (arrBit[arrIndex] >> i * 4 & 0x0000000F);
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
			// trường hợp đặc biệt MINQINT ko cần đổi vì sẽ bị tràn số
			if (temp == MINQINT) {}
			else
			{
				temp = temp - QInt(0, 0, 0, 1);
				temp = ~temp;
			}
		}
		for (int i = 0; i < 128; i++) {
			// lưu lại giá trị bit đầu
			int firstBit = temp.arrBit[0] < 0;
			temp = temp << 1;

			// lần lượt nhân 2 chuỗi result va cộng vào giá trị bit đầu
			for (int index = result.size() - 1; index >= 0; index--) {
				result[index] += result[index] - '0' + firstBit;

				firstBit = (result[index] > '9');

				if (firstBit) {
					result[index] -= 10;
				}
			}
		}

		while (result.size() > 1 && result[0] == '0') {
			// loại bỏ các kí tự '0' ở đầu chuỗi
			result.erase(result.begin());
		}

		if (isNegative == true) {
			// thêm dấu - nếu là số âm
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
		// Các trường hợp xuất hiện bit nhớ
		if (arrBit[i] < 0 && a.arrBit[i] >= 0 && temp >= 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0)
			isMissing = 1;
		if (arrBit[i] >= 0 && a.arrBit[i] < 0 && temp >= 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}

	// check overflow
	if (this->isPositive() && a.isPositive() && !result.isPositive())
		throw "OVERFLOW";
	if (!this->isPositive() && !a.isPositive() && result.isPositive())
		throw "OVERFLOW";

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
		// Các trường hợp xuất hiện bit nhớ
		if (arrBit[i] >= 0 && a.arrBit[i] >= 0 && temp < 0)
			isMissing = 1;
		if (arrBit[i] < 0 && a.arrBit[i] < 0 && temp < 0)
			isMissing = 1;
		if (arrBit[i] >= 0 && a.arrBit[i] < 0)
			isMissing = 1;
		result.arrBit[i] = temp;
	}

	// check overflow
	if (this->isPositive() && !a.isPositive() && !result.isPositive())
		throw "OVERFLOW";
	if (!this->isPositive() && a.isPositive() && result.isPositive())
		throw "OVERFLOW";

	return result;
}

QInt QInt::operator-(int a)
{
	return *this - QInt(0, 0, 0, a);
}

QInt QInt::operator*(QInt a)
{
	QInt result;

	// Kiểm tra nhân với 0
	if (this->isZero() || a.isZero())
		return result;

	// Kiểm tra phép nhân có số âm
	bool isNegative = false;
	if (a.arrBit[0] < 0 && this->arrBit[0] < 0) {
		isNegative = false;
	}
	else if (a.arrBit[0] < 0 || this->arrBit[0] < 0) {
		isNegative = true;
	}

	// Thuật toán Booth
	QInt A;
	QInt Q = *this;
	int Q1 = 0;
	QInt M = a;
	for (int i = 0; i < 128; ++i)
	{
		if (Q.getBitAt(0) == 1 && Q1 == 0)
			A = A - M;
		else if (Q.getBitAt(0) == 0 && Q1 == 1)
			A = A + M;
		int lsb_A = A.getBitAt(0);
		int lsb_Q = Q.getBitAt(0);
		A = A >> 1;
		Q = Q >> 1;

		Q.setBitAt(127, lsb_A);

		Q1 = lsb_Q;
	}
	result = Q;

	// Kiểm tra kết quả hợp lệ trước khi trả về
	if (isNegative == true && (A == QInt(-1, -1, -1, -1)) && !result.isPositive())
		return  result;
	else if (isNegative == false && (A == QInt(0, 0, 0, 0)) && result.isPositive())
		return result;
	else
		throw "OVERFLOW";
}

QInt QInt::operator/(QInt a)
{
	// Một trong 2 số bằng 0
	if (a.isZero())
		throw "DIVISION BY 0";
	if (this->isZero())
		return QInt(0, 0, 0, 0);

	QInt result;
	QInt replace = *this;

	// Kiểm tra phép chia có số âm và chuyển về số dương hết
	bool isNegative = false;
	if (a.arrBit[0] < 0 && this->arrBit[0] < 0) {
		isNegative = false;
		// Nếu bằng MINQINT thì giữ nguyên
		if (*this != MINQINT)
			replace = QInt(0, 0, 0, 0) - *this;
		if (a != MINQINT)
			a = QInt(0, 0, 0, 0) - a;
	}
	else if (a.arrBit[0] < 0 || this->arrBit[0] < 0) {
		isNegative = true;
		// Nếu bằng MINQINT thì giữ nguyên
		if (a.arrBit[0] < 0) {
			if (a != MINQINT)
				a = QInt(0, 0, 0, 0) - a;
		} else {
			if (*this != MINQINT)
				replace = QInt(0, 0, 0, 0) - *this;
		}
	}

	//số chia và số bị chia bằng nhau
	if (replace == a)
	{
		QInt temp("1", 10);
		if (isNegative) temp = QInt(-1, -1, -1, -1);
		return temp;
	}
	// TH đặc biệt: số chia = MINQINT lớn hơn số bị chia
	if (a == MINQINT)
		return QInt(0, 0, 0, 0);
	// số chia lớn hơn số bị chia
	if (replace != MINQINT && (replace - a).isPositive() == false)
		return QInt(0, 0, 0, 0);
	// TH đặc biệt khiến thuật toán chia báo lỗi tràn
	if (replace == MINQINT && (a - QInt(0x40000000, 0, 0, 1)).isPositive())
	{
		QInt tmp(0, 0, 0, 1);
		if (isNegative) tmp = QInt(-1, -1, -1, -1);
		return tmp;
	}

	// Thuật toán Restoring Divison (cho 2 số dương)
	QInt A;
	QInt Q = replace;
	QInt M = a;

	for (int i = 0; i < 128; ++i)
	{
		int lsb_Q = Q.getBitAt(127);
		A = A << 1;
		Q = Q << 1;
		A.setBitAt(0, lsb_Q);

		A = A - M;
		if (!A.isPositive())
		{
			Q.setBitAt(0, 0);
			A = A + M;
		}
		else
			Q.setBitAt(0, 1);
	}

	// trường hợp MINQINT x -1 = -MINQINT gây tràn
	if (Q == MINQINT && !isNegative)
		throw "OVERFLOW";

	result = Q;
	if (isNegative == true && result != MINQINT)
		result = QInt(0, 0, 0, 0) - Q;
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
				// Gán giá trị 0 tại vị trí i nếu vị trí i - a là bit 0
				result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
			}
			else {
				// Gán giá trị 1 tại vị trí i nếu vị trí i - a là bit i
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
				// Gán giá trị 0 tại vị trí i nếu vị trí i + a là bit 0
				result.arrBit[getArrayIndex(i)] = ~(1 << getBitIndex(i)) & result.arrBit[getArrayIndex(i)];
			}
			else {
				// gán giá trị 1 tại vị trí i nếu vị trí i + a là bit 1
				result.arrBit[getArrayIndex(i)] = (1 << getBitIndex(i)) | result.arrBit[getArrayIndex(i)];
			}
		}

		for (int i = 127; i >= 0 && i > 127 - a; i--) {
			// gán cho a kí tự đầu giá trị của bit dấu
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

bool QInt::operator==(QInt a)
{
	return arrBit[0] == a.arrBit[0] and
		arrBit[1] == a.arrBit[1] and
		arrBit[2] == a.arrBit[2] and
		arrBit[3] == a.arrBit[3];
}

bool QInt::operator!=(QInt a)
{
	return !(*this == a);
}

QInt QInt::rol()
{
	QInt result;
	QInt temp;
	int bit = getBitAt(127);	// lấy msb
	temp = *this << 1;			// dịch trái
	temp.setBitAt(0, bit);		// set lsb thành msb vừa lấy ở trên
	result = temp;
	return result;
}

QInt QInt::ror()
{
	QInt result;
	QInt temp;
	int bit = getBitAt(0);		// lấy lsb
	temp = *this >> 1;			// dịch phải
	temp.setBitAt(127, bit);	// set msb thành lsb vừa lấy ở trên
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
	// Lấy giá trị của bit (0 hoặc 1) tại vị trí index
	int result;
	int position_byte = (127 - index) / 32;
	int position_bit = index % 32;
	result = (this->arrBit[position_byte] >> position_bit) & 1;
	return result;
}

void QInt::setBitAt(int index, int value)
{
	// Gán giá trị của bit tại vị trí index bằng giá trị value
	int position_byte = (127 - index) / 32;
	int position_bit = index % 32;
	if (value == 1)
		this->arrBit[position_byte] = (1 << position_bit) | this->arrBit[position_byte];
	else if (value == 0)
		this->arrBit[position_byte] = ~(1 << position_bit) & this->arrBit[position_byte];
}

const QInt QInt::MINQINT(0x80000000, 0, 0, 0);