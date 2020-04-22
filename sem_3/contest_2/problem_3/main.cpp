#include <iostream>
#include <vector>
#include <cmath>

const unsigned int ALF_SIZE = 256;
const char SEPARATOR_1 = '#';       // separator between str1 and str2
const char SEPARATOR_2 = '$';       // separator in the end of str


class smart_str {
public:
    smart_str(std::string &str, int _str1_len);

    int count_substrings(std::string &str);

    std::pair<int, int> count_of_substring(int64_t k);

private:

    std::vector<int> build_suffix_array(std::string &str);

    //  классы эквивалентности(матрица классов от начало построения до конечного класса)
    std::vector<std::vector<int>> classes;
    std::vector<int> suff_array;

    std::vector<int> lcp;       // max len of common prefix, of two near suffix

    unsigned int len_str;
    unsigned int len_str1;
};

smart_str::smart_str(std::string &str, int _str1_len) {
    classes.assign(1, std::vector<int>(str.length()));
    suff_array = build_suffix_array(str);
    lcp.assign(suff_array.size(), 0);
    len_str = str.length();
    len_str1 = _str1_len;
    count_substrings(str);      // to fill lcp
};

std::vector<int> smart_str::build_suffix_array(std::string &str) {

    //  помечаем конец строки
    const unsigned int str_len = str.length();
    std::vector<int> permutations(str_len, 0);
    classes.clear();
    classes.assign(1, std::vector<int>(str_len));

    // сортируем подстроки из одного элемента подсчетом
    std::vector<int> count(ALF_SIZE, 0);
    for (char i : str)
        ++count[i];
    for (int i = 1; i < ALF_SIZE; i++)
        count[i] += count[i - 1];
    for (int i = 0; i < str_len; i++)
        permutations[--count[str[i]]] = i;

    // считаем количество классов эквивалентности для строк из двух символов (class[0])
    classes[0][permutations[0]] = 0;
    int classes_num = 1;
    for (int i = 1; i < str_len; i++) {
        if (str[permutations[i]] != str[permutations[i - 1]])
            ++classes_num;
        classes[0][permutations[i]] = classes_num - 1;
    }

    //   содержит перестановку в порядке сортировки по вторым элементам пар
    std::vector<int> new_permutations(str_len, 0);
    // повторяем пока длина подстрок ( 2^(j + 1) ) не превышает длины строки
    for (int j = 0; (1 << j) < str_len; j++) {
        for (int i = 0; i < str_len; i++) {
            new_permutations[i] = permutations[i] - (1 << j);
            if (new_permutations[i] < 0) {
                new_permutations[i] += str_len;
            }
        }
        count.assign(classes_num, 0);
        for (int i = 0; i < str_len; i++) {
            ++count[classes[j][new_permutations[i]]];
        }
        for (int i = 1; i < classes_num; i++) {
            count[i] += count[i - 1];
        }
        for (int i = str_len - 1; i >= 0; i--) {    // можт не влезть
            permutations[--count[classes[j][new_permutations[i]]]] = new_permutations[i];
        }

        // пересчитываем новые классы эквивалентности
        classes.emplace_back(str_len);
        classes[j + 1][permutations[0]] = 0;
        classes_num = 1;
        for (int i = 1; i < str_len; i++) {
            int mid1 = (permutations[i] + (1 << j)) % str_len;
            int mid2 = (permutations[i - 1] + (1 << j)) % str_len;
            if (classes[j][permutations[i]] != classes[j][permutations[i - 1]] ||
                classes[j][mid1] != classes[j][mid2]) {
                classes_num++;
            }
            classes[j + 1][permutations[i]] = classes_num - 1;
        }
    }
    return permutations;
}

int smart_str::count_substrings(std::string &str) {
    const unsigned int len = suff_array.size();

    //  наибольший общий префикс двух подстрок, используя lcp
    std::vector<int> _lcp(len_str, 0);
    std::vector<int> suffix_inverse(len_str);
    for (int i = 0; i < len_str; i++)
        suffix_inverse[suff_array[i]] = i;
    int k_shift = 0;
    for (int i = 0; i < len_str; i++) {
        if (k_shift > 0)
            --k_shift;
        if (suffix_inverse[i] == len_str - 1) {
            _lcp[len_str - 1] = -1;
            k_shift = 0;
        } else {
            int j = suff_array[suffix_inverse[i] + 1];
            while (std::max(i + k_shift, j + k_shift) < len_str && str[i + k_shift] == str[j + k_shift])
                ++k_shift;
            _lcp[suffix_inverse[i]] = k_shift;
        }
    }

    std::copy(_lcp.begin(), _lcp.end(), lcp.begin());

    // количество различных подстрок
    int sub_str = 0;
    for (int i = 1; i < len; i++)
        sub_str += (len - 1) - suff_array[i];
    for (int i = 0; i < len - 2; i++)
        sub_str -= _lcp[i];


    return sub_str;
}

std::pair<int, int> smart_str::count_of_substring(int64_t k) {
    int64_t count_sub_str = 0;              // count of common substrings
    int64_t begin_of_substr = -1;           // begin of k common substring
    int64_t end_of_substr = -1;             // end of k common substring
    for (int i = 1; i < len_str; i++) {
        if (lcp[i] > lcp[i - 1] && ((suff_array[i] < len_str1 || suff_array[i + 1] < len_str1) &&
                                    (suff_array[i] > len_str1 || suff_array[i + 1] > len_str1))) {  // lcp from different str
            count_sub_str += lcp[i] - lcp[i - 1];
        } else {
            if (lcp[i] > lcp[i - 1]) {  // to correctly count lcp in future lcp from different str
                lcp[i] = lcp[i - 1];
            }
        }
        if (count_sub_str == k) {       // our k common substring
            begin_of_substr = suff_array[i];
            end_of_substr = suff_array[i] + lcp[i] - count_sub_str + k;
            break;
        }
    }
    std::pair<int, int> res;
    if (begin_of_substr == -1)
        res = std::pair<int, int>(-1, -1);
    else
        res = std::pair<int, int>(begin_of_substr, end_of_substr);

    return res;
}


int main() {
    std::string str1, str2;
    std::cin >> str1 >> str2;

    int64_t k = 0;
    std::cin >> k;

    std::string str3 = str1 + SEPARATOR_1 + str2 + SEPARATOR_1;
    smart_str my_str(str3, str1.length());

    std::pair<int, int> sub_str = my_str.count_of_substring(k);
    if (sub_str.first == sub_str.second && sub_str.first == -1)
        std::cout << -1;
    else
        for (int i = sub_str.first; i < sub_str.second; ++i)
            std::cout << str3[i];

    return 0;
}