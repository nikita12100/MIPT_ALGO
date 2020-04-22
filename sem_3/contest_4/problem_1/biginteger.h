#ifndef A_BIGINTEGER_H
#define A_BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>

const unsigned char ASCII_POS_0 = '0';
const unsigned char ASCII_POS_MINUS = '-';


class BigInteger {
public:
    BigInteger(int a=0);    // non explicit
    BigInteger(std::string s);

    friend const BigInteger operator+(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator-(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator*(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator/(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator%(const BigInteger &a, const BigInteger &b);

    BigInteger &operator+=(const BigInteger &b);
    BigInteger &operator-=(const BigInteger &b);
    BigInteger &operator*=(const BigInteger &b);
    BigInteger &operator/=(const BigInteger &b);
    BigInteger &operator%=(const BigInteger &b);

    const BigInteger operator-();

    BigInteger &operator++();
    const BigInteger operator++(int);
    BigInteger &operator--();
    const BigInteger operator--(int);

    bool operator==(BigInteger const &b) const;
    bool operator!=(BigInteger const &b) const;
    bool operator<(BigInteger const &b) const;
    bool operator>(BigInteger const &b) const;
    bool operator<=(BigInteger const &b) const;
    bool operator>=(BigInteger const &b) const;

    friend std::istream &operator>>(std::istream &, BigInteger &);
    friend std::ostream &operator<<(std::ostream &, const BigInteger &);

    const std::string toString() const;

    explicit operator bool();

//private:
    std::vector<int> number;
    bool positive = true;

    BigInteger abs() const;

    void insertFirstNulll();

    void cleanZeros();

    void dif(BigInteger &a, const BigInteger &b);   // result store in a

    void sum(BigInteger &a, const BigInteger &b);   // result store in a

    int compare(const BigInteger &a, bool checkSgn = true) const; // 1 -- a>b, -1 a<b, 0 -- a=b
};


BigInteger::BigInteger(int a) {
    if (a < 0) {
        positive = false;
        a *= -1;
    } else if (a == 0) {
        number.push_back(0);
        positive = true;
    }
    //positive = true;
    while (a != 0) {
        number.push_back(a % 10);
        a /= 10;
    }
}

BigInteger::BigInteger(std::string s) {
    positive = true;
    if (s.length() == 0){
        number.push_back(0);
        return ;
    }
    for (unsigned int i = s.length() - 1; i > 0; i--)
        number.push_back(s[i] - ASCII_POS_0);
    if (s[0] == ASCII_POS_MINUS)
        positive = false;
    else
        number.push_back(s[0] - ASCII_POS_0);
    if (s.length() == 2 && s[1] == ASCII_POS_0)      // -0
        positive = true;
}


const BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result += b;
    return result;
}

const BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result -= b;
    return result;
}

const BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result *= b;
    return result;
}

const BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result /= b;
    return result;
}

const BigInteger operator%(const BigInteger &a, const BigInteger &b) {
    BigInteger result = a;
    result %= b;
    return result;
}


std::istream &operator>>(std::istream &str, BigInteger &a) {
    std::string s;
    str >> s;
    BigInteger b(s);
    a = b;
    return str;
}

std::ostream &operator<<(std::ostream &str, const BigInteger &a) {
    str << a.toString();
    return str;
}


bool BigInteger::operator<(BigInteger const &b) const {
    return compare(b) == -1;
}

bool BigInteger::operator>(BigInteger const &b) const {
    return compare(b) == 1;
}

bool BigInteger::operator<=(BigInteger const &b) const {
    int comp = compare(b);
    return comp <= 0;
}

bool BigInteger::operator>=(BigInteger const &b) const {
    int comp = compare(b);
    return comp >= 0 ;
}

bool BigInteger::operator==(BigInteger const &b) const {
    return compare(b) == 0;
}

bool BigInteger::operator!=(BigInteger const &b) const {
    return compare(b) != 0;
}


BigInteger &BigInteger::operator*=(const BigInteger &b) {
    unsigned int length = (*this).number.size() + b.number.size() + 1;
    BigInteger result;
    result.number.resize(length, 0);

    for (unsigned int i = 0; i < (*this).number.size(); i++)
        for (unsigned int j = 0; j < b.number.size(); j++)
            result.number[i + j] += (*this).number[i] * b.number[j];

    for (unsigned int i = 0; i < length; i++) {
        if (i < length - 1)
            result.number[i + 1] += result.number[i] / 10;  // carry
        result.number[i] %= 10;
    }

    result.positive = (*this).positive == b.positive;
    result.cleanZeros();
    (*this) = result;
    return (*this);
}

BigInteger &BigInteger::operator+=(const BigInteger &b) {
    if ((positive && b.positive) || (!positive && !b.positive)) {
        sum(*this, b);
        return *this;
    }
    if ((positive && !b.positive) || (!positive && b.positive)) {
        if (this->compare(b, false) >= 0) {
            dif(*this, b);
            return *this;
        } else {
            BigInteger result = b;
            dif(result, *this);
            *this = result;
            return *this;
        }
    }
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &b) {
    if ((positive && b.positive) || (!positive && !b.positive)) {
        if (this->compare(b, false) >= 0) {
            dif(*this, b);
            return *this;
        } else {
            BigInteger result = b;
            dif(result, *this);
            result.positive = !result.positive;
            return (*this = result);
        }
    } else {
        sum(*this, b);
        return *this;
    }
}

BigInteger &BigInteger::operator%=(const BigInteger &b) {
    BigInteger res;
    res.number.resize((*this).number.size());
    BigInteger curValue;
    for (int i = int((*this).number.size()) - 1; i >= 0; i--) {
        curValue.insertFirstNulll();
        curValue.cleanZeros();
        curValue.number[0] = (*this).number[i];
        int x = 0;
        int l = 0, r = 9;
        while (l <= r) {
            int m = (l + r) >> 1;
            BigInteger cur;
            cur = b.abs() * m;
            if (cur.compare(curValue, false) <= 0) {
                x = m;
                l = m + 1;
            } else
                r = m - 1;
        }
        res.number[i] = x;
        curValue = curValue - b.abs() * x;
    }

    curValue.cleanZeros();
    curValue.positive = (*this).positive;
    if (curValue.number.empty())
        curValue.number.push_back(0);
    (*this) = curValue;
    return (*this);
}

BigInteger &BigInteger::operator/=(const BigInteger &b) {
    BigInteger res;
    res.number.resize((*this).number.size(), 0);
    BigInteger curValue;

    for (int i = int((*this).number.size()) - 1; i >= 0; i--) {
        curValue.insertFirstNulll();
        curValue.cleanZeros();
        curValue.number[0] = (*this).number[i];
        int x = 0;
        int l = 0, r = 9;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger cur(0);
            cur = b.abs() * m;
            if (cur.compare(curValue, false) <= 0) {
                x = m;
                l = m + 1;
            } else
                r = m - 1;
        }
        res.number[i] = x;
        curValue -= b.abs() * x;
    }

    res.positive = (*this).positive == b.positive;
    res.cleanZeros();
    (*this) = res;
    return (*this);
}


BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

const BigInteger BigInteger::operator++(int) {
    BigInteger new_this = *this;
    *this += 1;
    return new_this;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

const BigInteger BigInteger::operator--(int) {
    BigInteger buff = *this;
    (*this) -= 1;
    return buff;
}


BigInteger BigInteger::abs() const {
    BigInteger b = *this;
    b.positive = true;
    return b;
}


const BigInteger BigInteger::operator-() {
    BigInteger b = *this;
    b.positive = !this->positive;
    if (number.size() == 1 && number[0] == 0) {
        b.positive = true;
        b.number[0] = 0;
        return b;
    }
    return b;
}


void BigInteger::insertFirstNulll() {
    number.insert(number.begin(), 0);
}


const std::string BigInteger::toString() const {
    std::string s;
    if(!positive)
        s = ASCII_POS_MINUS;
    if (number[0] == 0 && number.size() == 1) {
        s = ASCII_POS_0;
        return s;
    }
    bool skipNuls = true;
    for (int i = int(number.size()) - 1; i >= 0; i--) {
        if (number[i] == 0 && skipNuls) {
            continue;
        } else {
            skipNuls = false;
            s += number[i] + ASCII_POS_0;
        }
    }
    if (s.empty() || (s[0] == ASCII_POS_MINUS && s.length() == 1))
        s = ASCII_POS_0;
    return s;
}


BigInteger::operator bool() {
    return *this != 0;
}


void BigInteger::cleanZeros() {
    while (!number.empty() && *number.rbegin() == 0) {
        number.pop_back();
    }
    if (number.empty()) {
        positive = true;
        number.push_back(0);
    }
}


void BigInteger::dif(BigInteger &a, const BigInteger &b) {
    unsigned int carry = 0;
    for (unsigned int i = 0; i < b.number.size() || carry; i++) {
        a.number[i] -= carry + (i < b.number.size() ? b.number[i] : 0);
        carry = (a.number[i] < 0) ? 1 : 0;
        if (carry == 1)
            a.number[i] += 10;
    }
    a.cleanZeros();
}

void BigInteger::sum(BigInteger &a, const BigInteger &b) {
    unsigned int carry = 0;
    for (unsigned int i = 0; i < std::max(a.number.size(), b.number.size()) || carry; i++) {
        if (i == a.number.size())
            a.number.push_back(0);
        a.number[i] += carry + (i < b.number.size() ? b.number[i] : 0);
        carry = a.number[i] / 10;
        a.number[i] %= 10;
    }
    a.cleanZeros();
}


int BigInteger::compare(const BigInteger &a, bool checkSgn) const {
    int check = 1;
    if(checkSgn) {
        if (this->positive && !a.positive)
            return 1;
        if (!this->positive && a.positive)
            return -1;
        if (!this->positive && !a.positive)
            check = -1;
    }

    if (this->number.size() != a.number.size())
        return (this->number.size() < a.number.size()) ? (-1 * check) : check;

    for (int i = this->number.size(); i > 0; i--) {
        if (this->number[i - 1] < a.number[i - 1])
            return (-1 * check);
        if (this->number[i - 1] > a.number[i - 1])
            return check;
    }
    return 0;
}

#endif //A_BIGINTEGER_H