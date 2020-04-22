#include <iostream>
#include <vector>
#include <cmath>

const unsigned int ALF_SIZE = 256;

class smart_str {
public:
    explicit smart_str(std::string &str);       /// can change !!

    int count_substrings();

private:

    std::vector<int> build_suffix_array(std::string &str);

    //  классы эквивалентности(матрица классов от начало построения до конечного класса)
    std::vector<std::vector<int>> classes;
    std::vector<int> suff_array;

};

smart_str::smart_str(std::string &str) {
    classes.assign(1, std::vector<int>(str.length()));
    suff_array = build_suffix_array(str);
};


std::vector<int> smart_str::build_suffix_array(std::string &str) {

    //  помечаем конец строки
    str.push_back('\0');
    const unsigned int str_len = str.length();
    std::vector<int> permutations(str_len, 0);
    ///classes.clear();
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


    if(classes_num = str_len){
        for(int k=0; k < str_len; k++)
            std::cout << classes[0][k] << ' ';
        return classes[0];
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
    str.pop_back(); // конец стоки \0
    return permutations;
}

int smart_str::count_substrings() {
    const unsigned int len = suff_array.size();

    //  наибольший общий префикс двух подстрок, используя lcp
    std::vector<int> lcp(len - 2, 0);
    for (int i = 1; i < len - 1; i++) {
        int left = suff_array[i];
        int right = suff_array[i + 1];
        for (int k = (int) log(len); k >= 0; k--)
            if (classes[k][left] == classes[k][right]) {
                lcp[i - 1] += 1 << k;
                left += 1 << k;
                right += 1 << k;
            }
    }

    // количество различных подстрок
    int sub_str = 0;
    for (int i = 1; i < len; i++)
        sub_str += (len - 1) - suff_array[i];
    for (int i = 0; i < len - 2; i++)
        sub_str -= lcp[i];

    return sub_str;
}

int main() {
    std::string str;
    std::cin >> str;

    smart_str my_str(str);
    std::cout << my_str.count_substrings();

    return 0;
}