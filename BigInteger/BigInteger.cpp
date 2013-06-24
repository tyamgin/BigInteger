#include <string>
#include <string.h>
#include <algorithm>
#include <iostream>

/*

	http://www.e-olimp.com/problems/313  A+B,   0 <= A,B <= 10^500
	http://www.e-olimp.com/problems/265  A+B    0 <= A,B <= 10^10000
	http://www.e-olimp.com/problems/266  A<>B   0 <= A,B <= 10^10000
	http://www.e-olimp.com/problems/267  A-B    0 <= A,B <= 10^10000
	http://www.e-olimp.com/problems/271  n!     0 <= n <= 3000
	http://www.e-olimp.com/problems/272  A*B    0 <= A,B <= 10^10000
	http://www.e-olimp.com/problems/314  A+-B   0 <= A,B <= 10^500
	http://www.e-olimp.com/problems/317  A*B    0 <= A,B <= 10^195000
	http://www.e-olimp.com/problems/1655 lcm(1, 2, ..., n)  1 <= n <= 1000
	http://www.e-olimp.com/problems/2618 A+1    0 <= A <= 1e6
	http://www.e-olimp.com/problems/1328
	http://www.e-olimp.com/problems/2625 BigDecimal
*/

// **************************************************************<BigInteger>**************************************************************************************** //

class BigInteger
{
	int *a;
	int size;
	int capacity;
	bool minus;

	static const int BASE;
	static const int BASE_DIGITS;

	void reduce() {
		while(size > 1 && a[size - 1] == 0)
			size--;
		if (isZero())
			minus = false;
	}

	void alloc(int amount) {
		capacity = amount * 2;
		a = new int[capacity];
	}

	void ensureCapacity(int amount) {	
		if (amount > capacity) {
			capacity = amount * 2;
			a = (int*)realloc(a, capacity * sizeof(int));
		}
	}

public:

	bool isZero() const {
		return size == 1 && a[0] == 0;
	}

	int getSign() const {
		if (isZero())
			return 0;
		return minus ? -1 : 1;
	}

	BigInteger abs() const {
		return minus ? -*this : *this;
	}

	BigInteger() {
		this->alloc(1);
	    size = 1;
	    a[0] = 0;
	}

	explicit BigInteger(const std::string& s) {
		this->alloc(s.size() / BASE_DIGITS + 1);
		size = 0;
		minus = s[0] == '-';
		int lf = int(minus);
		if (s[0] == '+')
			lf = 1;

		for(int i = (int)s.size() - 1; i >= lf; i -= BASE_DIGITS) {
			int digit = 0;
			for(int j = std::max(lf, i - BASE_DIGITS + 1); j <= i; j++)
				digit = digit * 10 + s[j] - '0';
			a[size++] = digit;
		}
		reduce();
	}

	explicit BigInteger(long long x) {
		this->alloc(3);
		minus = false;
		if (x < 0) {
			minus = true;
			x = -x;
		}
		int i;
		for(i = 0; x != 0; i++) {
			a[i] = int(x % BASE);
			x /= BASE;
		}
		size = std::max(1, i);
		reduce();
	}

	BigInteger(const BigInteger &p) {
		this->alloc(p.size);
		size = p.size;
		minus = p.minus;
		memcpy(a, p.a, size * sizeof(int));
	}

	BigInteger &operator =(const BigInteger &p) {
		ensureCapacity(p.size);
		size = p.size;
		minus = p.minus;
		memcpy(a, p.a, size * sizeof(int));
		return *this;
	}

	std::string toString() const {
		char buf[20];
		sprintf(buf, "%d", a[size - 1]);
		std::string res(buf);
		char fmt[] = "%0*d";
		fmt[2] = BASE_DIGITS + '0';
		for(int i = size - 2; i >= 0; i--) {
			sprintf(buf, fmt, a[i]);
			res += buf;
		}
		if (minus)
			res = "-" + res;
		return res;
	}

	long long toLongLong() const {
		long long res = 0;
		for(int i = size - 1; i >= 0; i--)
			res = res * BASE + a[i];
		return minus ? -res : res;
	}

	BigInteger operator-() const {
		BigInteger res(*this);
		res.minus ^= true;
		res.reduce();
		return res;
	}

	void operator +=(const BigInteger& p) {
		if (p.isZero())
			return;
		if (isZero()) {
			*this = p;
			return;
		}
		if (minus != p.minus) {
			*this -= -p;
			return;
		}
		int i;
		int carry = 0;
		ensureCapacity(std::max(size, p.size) + 1);
		for(i = 0; i < p.size || carry != 0; i++) {
			carry += (i < size ? a[i] : 0) + (i < p.size ? p.a[i] : 0);
			int ncarry = 0;
			while (carry >= BASE) {
		        carry -= BASE;
				ncarry++;
			}
			a[i] = carry;
			carry = ncarry;
		}
		size = std::max(size, i);
		reduce();
	}

	void operator -=(const BigInteger& p) {
		if (p.isZero())
			return;
		if (isZero()) {
			*this = -p;
			return;
		}
		if (minus != p.minus) {
			*this += -p;
			return;
		}

		size = std::max(size, p.size);
		ensureCapacity(size + 1);
		int i;
		int carry = 0;
		for (i = 0; (i < p.size || carry != 0) && i < size; i++) {
			carry += a[i] - (i < p.size ? p.a[i] : 0);
			int ncarry = 0;
			while (carry < 0) {
				carry += BASE;
				ncarry--;
			}
			a[i] = carry;
			carry = ncarry;
		}
		if (carry != 0)
			a[size++] = carry;

		if (a[size - 1] < 0) {
			for(int j = 0; j < size - 1; j++)
				a[j] = BASE - 1 - a[j];
			a[size - 1] = 0;
			for (i = 0; ; i++)
				if (++a[i] < BASE)
					break;
				else
					a[i] = 0;
			size = std::max(size, i + 1);
			minus ^= true;
		}
		reduce();
	}

	void operator *=(int x) {
		if (x < 0) {
			minus ^= true;
			x = -x;
		}
		int i;
		long long carry = 0;
		ensureCapacity(size + 2);
		for (i = 0; i < size || carry != 0; i++) {
			carry += i < size ? 0 : a[i] * long long(x);
			a[i] = int(carry % BASE);
			carry /= BASE;
		}
		size = std::max(size, i);
		reduce();
	}

	void operator /= (int x) {
	    if (x == 0) {
			std::cerr << "Dividing by zero" << std::endl;
			throw 1;
		}
		if (x >= BASE)
			return operator /=(*new BigInteger(x));
		if (x < 0) {
			minus ^= true;
			x = -x;
		}
		long long carry = 0;
		for(int i = size - 1; i >= 0; i--) {
			carry = carry * BASE + a[i];
			a[i] = int(carry / x);
			carry %= x;
		}
		reduce();
	}

	int operator %(int x) const {
		if (x <= 0) {
			std::cerr << "Mod by value <= 0" << std::endl;
			throw 1;
		}
		int carry = 0;
		for(int i = size - 1; i >= 0; i--)
			carry = (carry * long long(BASE) + a[i]) % x;
		if (minus && carry != 0)
			carry = x - carry;
		return carry;
	}

	BigInteger operator *(const BigInteger& b) const {
	    BigInteger res;
		res.ensureCapacity(size + b.size + 1);
		memset(res.a, 0, (size + b.size + 1) * sizeof(int));
	    res.minus = minus ^ b.minus;
		for(int i = 0; i < size; i++) {
			int j;
			long long carry = 0;
			for(j = 0; j < b.size || carry != 0; j++) {
				carry += res.a[i + j] + (j < b.size ? a[i] * long long(b.a[j]) : 0);
				res.a[i + j] = int(carry % BASE);
				carry /= BASE;
			}
			res.size = std::max(res.size, i + j);
		}
		res.reduce();
		return res;
	}

	int compareTo(const BigInteger& p) const {
		int lf = -1, rg = +1;
		if (minus) {
			if (!p.minus)
		        return -1;
			std::swap(lf, rg);
		}
		if (size != p.size)
			return size < p.size ? lf : rg;
		for(int i = size - 1; i >= 0; i--)
			if (a[i] != p.a[i])
				return a[i] < p.a[i] ? lf : rg;
		return 0;
	}

	std::pair<BigInteger, BigInteger> divide(BigInteger b) const {
		if (b.isZero()) {
			std::cerr << "Dividing by zero" << std::endl;
			throw 1;
		}
		BigInteger a = *this;
		BigInteger res;
		res.ensureCapacity(size);
		bool res_minus = a.minus ^ b.minus;
		a.minus = b.minus = false;
		int diff = std::max(0, (a.size - b.size - 1) * BASE_DIGITS);
		BigInteger pw10("1" + std::string(diff, '0'));
		b *= pw10;
		while (b <= a) {
			BigInteger nb = b * 10;
			if (nb > a)
				break;
			b = nb;
			pw10 *= 10;
		}
		while (!pw10.isZero()) {
			while (b <= a) {
				a -= b;
				res += pw10;
			}
			b /= 10;
			pw10 /= 10;
		}
		res.minus = res_minus;
		res.reduce();
		return std::make_pair(res, a);
	}

	BigInteger operator +(const BigInteger& b) const {
		BigInteger res(*this);
		res += b;
		return res;
	}

	BigInteger operator -(const BigInteger& b) const {
		BigInteger res(*this);
		res -= b;
		return res;
	}

	BigInteger operator *(int x) const {
		BigInteger res(*this);
		res *= x;
		return res;
	}

	void operator *=(const BigInteger& b) {
		*this = *this * b;
	}

	BigInteger operator /(int x) const {
	    BigInteger res(*this);
	    res /= x;
	    return res;
	}

	void operator %=(int x) {
	    *this = BigInteger(*this % x);
	}

	void operator /=(const BigInteger& p) {
	    *this = divide(p).first;
	}

	BigInteger operator /(const BigInteger& p) const {
		return divide(p).first;
	}

	void operator %=(const BigInteger& p) {
		*this = divide(p).second;
	}

	BigInteger operator %(const BigInteger& p) const {
		return divide(p).second;
	}

	bool operator ==(const BigInteger& p) const {
		return compareTo(p) == 0;
	}

	bool operator !=(const BigInteger& p) const {
	    return compareTo(p) != 0;
	}

	bool operator <(const BigInteger& p) const {
		return compareTo(p) < 0;
	}

	bool operator <=(const BigInteger& p) const {
	    return compareTo(p) <= 0;
	}

	bool operator >(const BigInteger& p) const {
	    return compareTo(p) > 0;
	}

	bool operator >=(const BigInteger& p) const {
	    return compareTo(p) >= 0;
	}

	void operator ++(int) {
		operator +=(ONE);
	}

	void operator --(int) {
		operator -=(ONE);
	}

	static const BigInteger ONE;
	static const BigInteger ZERO;
	static const BigInteger TEN;
	static const BigInteger MINUS_ONE;

	friend class std::pair<BigInteger, BigInteger>;
};

std::ostream &operator <<(std::ostream &out, BigInteger p) {
	return out << p.toString();
}

std::istream &operator >>(std::istream &in, BigInteger &p) {
	std::string s;
	in >> s;
	p = BigInteger(s);
	return in;
}

const int BigInteger::BASE = (int)1e9;
const int BigInteger::BASE_DIGITS = 9;

const BigInteger BigInteger::ONE = BigInteger(1LL);
const BigInteger BigInteger::ZERO = BigInteger(0LL);
const BigInteger BigInteger::TEN = BigInteger(10LL);
const BigInteger BigInteger::MINUS_ONE = BigInteger(-1LL);

// **************************************************************</BigInteger>**************************************************************************************** //

using namespace std;
#include <time.h>

//void divideIntTest()
//{
//	clock_t timer = clock();
//	BigInteger a(string("1") + string(10000000, '0'));
//	int b = 999999999;
//	a /= b;
//	cout << "/(int)" << " " << clock() - timer << "ms" << endl;
//}
//
//void divideBigIntTest()
//{
//	clock_t timer = clock();
//	BigInteger a(string("1") + string(10000000, '0'));
//	BigInteger b(999999999);
//	a /= b;
//	cout << "/(int)" << " " << clock() - timer << "ms" << endl;
//}
//
//int main()
//{
//	divideIntTest();
//	divideBigIntTest();
//}

void multiplySolve()
{
	BigInteger a, b;
	cin >> a >> b;
	cout << a * b << endl;
}

int main()
{
	multiplySolve();
}