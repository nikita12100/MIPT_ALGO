#include <iostream>
#include <vector>

const char  QUESTION =  '?';
const std::string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
unsigned const int ALF_SIZE = 26;
// 26 = размер алфавита a-z

class aho_corasik_automat {      //   Автомат Ахо-Корасик, по подшаблонам без вопросов
public:
    explicit aho_corasik_automat(const std::string &patt);

    std::vector<int> find_matches(const std::string &text);   // позиции вхождения шаблона в тексте.

private:
    struct vertex {
        vertex(int parent, char parent_char);

        int edges[ALF_SIZE];     // массив сыновей
        int fast_move[ALF_SIZE];    // массив переходов (запоминаем переходы в ленивой рекурсии)
        int suffix_link;     // суффиксная ссылка из данной вершины (вычисляем в ленивой рекурсии)
        int parent;     // родительская вершина
        char parent_char;    // символ на ребре родитель - данная вершина
        bool is_pattern;   // флаг, является ли вершина концом шаблона
        std::vector<int> indices_of_pattern;    // номера строк, за которые отвечает терминал
    };

    void add_pattern(const std::string &patt, const std::pair<int, int> &submask_pos, int patt_index);

    void find_sub_pattern_pos(const std::string &patt);

    int get_suffix_link(int index);
    int get_link(int index, char character);

    unsigned int pattern_len;
    std::vector<vertex> bohr;    //  вершины бора
    std::vector<std::pair<int, int>> subpatt_positions;    // пары начало-конец подшаблона без символов ? в маске
};

aho_corasik_automat::vertex::vertex(int parent, char parent_char)
        : parent_char(parent_char),
          parent(parent),
          suffix_link(-1),
          is_pattern(false) {
    for (int j = 0; j < ALF_SIZE; j++) {
        edges[j] = -1;
        fast_move[j] = -1;
    }
}

aho_corasik_automat::aho_corasik_automat(const std::string &patt)
        : bohr(1, vertex(0, -1)),
          pattern_len(patt.length()) {
    bohr[0].suffix_link = 0;

    find_sub_pattern_pos(patt);

    for (int i = 0; i < subpatt_positions.size(); i++) {
        add_pattern(patt, subpatt_positions[i], i);   // поместили шаблон в бор
    }
}


void aho_corasik_automat::add_pattern(const std::string &patt, const std::pair<int, int> &submask_pos, int patt_index) {
    int current_vertex = 0;

    for (int i = submask_pos.first; i <= submask_pos.second; i++) {
        char character = patt[i] - ALPHABET[0];
        if (bohr[current_vertex].edges[character] == -1) {
            bohr.emplace_back(current_vertex, character);
            bohr[current_vertex].edges[character] = (int) (bohr.size() - 1);
        }
        current_vertex = bohr[current_vertex].edges[character];
    }
    bohr[current_vertex].is_pattern = true;
    bohr[current_vertex].indices_of_pattern.push_back(patt_index);
}


int aho_corasik_automat::get_suffix_link(int index) {
    if (bohr[index].suffix_link == -1) {   // если ссылки нет
        if (bohr[index].parent == 0) {  // елси это сын корня, ты ссылку указываем на корень
            bohr[index].suffix_link = 0;
        } else {                // иначе рекурсивно
            bohr[index].suffix_link =
                    get_link(get_suffix_link(bohr[index].parent), bohr[index].parent_char);
        }
    }
    return bohr[index].suffix_link;
}

void aho_corasik_automat::find_sub_pattern_pos(const std::string &patt) {
    std::pair<int, int> current_subpattern_pos;      // нач и конец шаблона между ?...?

    if (isalpha(patt[0])) {             // a-z, A-Z ?
        current_subpattern_pos.first = 0;
    }
    if (patt[1] == QUESTION && isalpha(patt[0])) {
        current_subpattern_pos.second = 0;
        subpatt_positions.push_back(current_subpattern_pos);
    }

    for (int i = 1; i < pattern_len - 1; i++) {
        if (patt[i - 1] == QUESTION && isalpha(patt[i])) {
            current_subpattern_pos.first = i;
        }
        if (patt[i + 1] == QUESTION && isalpha(patt[i])) {
            current_subpattern_pos.second = i;
            subpatt_positions.push_back(current_subpattern_pos);
        }
    }

    if (patt[patt.length() - 2] == QUESTION && isalpha(patt[patt.length() - 1])) {
        current_subpattern_pos.first = (int) patt.length() - 1;
    }
    if (isalpha(patt[pattern_len - 1])) {
        current_subpattern_pos.second = (int) (patt.length() - 1);
        subpatt_positions.push_back(current_subpattern_pos);
    }
}


int aho_corasik_automat::get_link(int index, char character) {
    if (bohr[index].fast_move[character] == -1) {   // если нет перехода по этому символу
        if (bohr[index].edges[character] != -1) {   // если нет переходов вообще
            bohr[index].fast_move[character] = bohr[index].edges[character];
        } else if (index == 0) {        // значит корень
            bohr[index].fast_move[character] = 0;
        } else {        // посмотрим на суффиксные ссылки
            bohr[index].fast_move[character] = get_link(get_suffix_link(index), character);
        }
    }
    return bohr[index].fast_move[character];
}


std::vector<int> aho_corasik_automat::find_matches(const std::string &text) {
    std::vector<int> entries(text.length(), 0);
    int vertex = 0;
    // ищем вхождения всех подшаблонов и увеличиваем entries в индексе, соотв. началу маски.
    for (int i = 0; i < text.length(); i++) {
        vertex = get_link(vertex, text[i] - ALPHABET[0]);
        int curr_vertex = vertex;
        do {
            if (bohr[curr_vertex].is_pattern) {     // нашли нужный нам шаблон (его начало)
                for (int index : bohr[curr_vertex].indices_of_pattern) {
                    int start_index = i - subpatt_positions[index].second + subpatt_positions[index].first;     // позиция начала шаблона в тексте

                    if ((start_index - subpatt_positions[index].first >= 0) &&              // совпадение шаблона
                        (start_index - subpatt_positions[index].first + pattern_len - 1 < text.length()))
                        entries[start_index - subpatt_positions[index].first]++;
                }
            }
            curr_vertex = get_suffix_link(curr_vertex);
        } while (curr_vertex != 0);
    }

    // Если индекс равен количеству подшаблонов, он является вхождением подстроки (совпадают все подшаьблоны )
    std::vector<int> result;
    for (int i = 0; i < entries.size(); i++) {
        if (entries[i] == subpatt_positions.size()) {
            result.push_back(i);
        }
    }
    return result;
}

int main() {
    std::string pattern, text;
    std::cin >> pattern;
    std::cin >> text;

    aho_corasik_automat aho_corasik_aut(pattern);   // тут мы построим бор от pattern

    std::vector<int> positions = aho_corasik_aut.find_matches(text);
    for (int pos: positions) {
        std::cout << pos << " ";
    }

    return 0;
}