#include <iostream>
#include <string>
#include <vector>

int pref_func(const std::vector<int> &pi, const std::string &pattern, char new_c) {
    int k = pi.back();
    while (k > 0 && new_c != pattern[k])
        k = pi[k - 1];
    // нет совпадения, идем влево по шаблону
    if (new_c == pattern[k])
        k++;
    // это когда предыдущее значиние преф функц(не факт что самой ближайшей) + 1
    return k;
}

int main() {
    std::string pattern;
    char text_char;

    std::cin >> pattern;

    std::vector<int> pi_pattern(1, 0);
    for (int i = 1; i < pattern.length(); i++) {
        pi_pattern.emplace_back(pref_func(pi_pattern, pattern, pattern[i]));
    }
    // префикс функция от шаблона


    pi_pattern.push_back(0);
    // добавляем разделитель

    int curr_pos = 1;
    //указатель на текущюю позицию во входном тексте
    getchar();
    while ((text_char = getchar()) != '\n') {
        pi_pattern.emplace_back(pref_func(pi_pattern, pattern, text_char));

        if (pi_pattern.back() == pattern.length())
            std::cout << curr_pos - pattern.length() << " ";
        // сразу выводим
        curr_pos++;
    }

    return 0;
}