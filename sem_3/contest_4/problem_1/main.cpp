#include <iostream>
#include "biginteger.h"

int main() {
    BigInteger b1 = 20;
    BigInteger b2 = 4;
    std::cout << b1 + b2 << "\n";
    std::cout << b1++ << "\n";
    std::cout << ++b1 << "\n";
    std::cout << b1 - b2 << "\n";
    std::cout << b1 * b2 << "\n";
    std::cout << b1 / b2 << "\n";
    std::cout << b1 % 7 << "\n";
    BigInteger b3;
    std::cin >> b3;
    std::cout << b3 << "\n";
    std::cout << b3.positive << "\n";
}