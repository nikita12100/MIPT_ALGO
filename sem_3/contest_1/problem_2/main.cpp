#include <iostream>
#include <vector>
#include <string>


const std::string ALPHABET = "abcdefghijklmnopqrstuvwxyz";  // >= 2 letters

std::string build_from_prefix(const std::vector<int> &pi) {
    std::string str;
    str += ALPHABET[0];     //"a";
    std::string min_unused;     // = "b"; // минимальный еще не использованный
    min_unused += ALPHABET[1];
    for (int i = 1; i < pi.size(); i++) {
        if (pi[i] == 0) {
            // след. буква от предыдущего  в шаблоне должна отличаться
            str += min_unused.back()++;
            // инкрементируем, так как использовали эту букву
            min_unused += ALPHABET[1];
            // елси это след от pi[i]=0
        } else {
            str += str[pi[i] - 1];
            min_unused += min_unused[pi[i] - 1];
        }
    }

    return str;
}

int main() {
    std::vector<int> pref_func;
    while (std::cin) {
        int i = 0;
        std::cin >> i;
        pref_func.push_back(i);
    }
    pref_func.pop_back(); // когда случается EOF, все равно происходит одно считывание

    std::cout << build_from_prefix(pref_func);

    return 0;
}