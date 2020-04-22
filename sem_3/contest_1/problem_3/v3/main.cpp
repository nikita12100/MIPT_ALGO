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


bool equal_str(const std::string &pattern, const std::string &text, const int strart_pos, const int quest_count) {
    bool answer = false;
    for (int i = 0; i < pattern.length() - quest_count; i++) {
        if (strart_pos + i >= text.length())
            return false;
        if (pattern[i + quest_count] == '?' || pattern[i + quest_count] == text[strart_pos + i])
            answer = true;
        else
            return false;
    }
    return answer;
}

int main() {
    std::string pattern, text;

    std::cin >> pattern;
    std::cin >> text;

    int quest_count = 0;
    // хотим посчитать все вхождения полстроки до знака ?, чтобы потом шаблон "откладывать" от неё
    std::string pref;                // то что между ?x?
    for (int k = 0; k < pattern.length(); k++) {
        if (pattern[k] == '?' && pref.length() != 0)
            break;
        if (pattern[k] == '?' && pref.length() == 0)    // когда text вида ?..?xxx , где pattern_stat - кол-во ?
            quest_count++;
        if (pattern[k] != '?')
            pref += pattern[k];
    }

    if (quest_count == pattern.length()) {                // вариант когда все text = ?..?
        for (int i = 0; i <= text.length() - pattern.length(); i++)
            std::cout << i << ' ';
        return 0;
    }


    std::vector<int> pi_text(1, 0);
    for (int i = 1; i < pref.length(); i++)     // префикс функция шаблона кот между первыми ?x?
        pi_text.emplace_back(pref_func(pi_text, pref, pref[i]));

    pi_text.emplace_back(0);
    int curr_pos = 1;
    for (auto c:text) {
        pi_text.emplace_back(pref_func(pi_text, pref, c));
        if (pi_text.back() == pref.length())                         //предпологаем что можем отложить отсюда
            if (equal_str(pattern, text, curr_pos - pref.length(), quest_count))     //проверяем может ли отложить
                std::cout << curr_pos - pref.length() - quest_count << ' ';

        curr_pos++;
    }

    return 0;
}