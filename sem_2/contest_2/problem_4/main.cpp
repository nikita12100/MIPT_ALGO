#include <vector>
#include <assert.h>
#include <list>
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>

using std::vector;
using std::list;
using std::string;
using std::set;
using std::map;
using std::pair;
using std::make_pair;


// храним сетап в ul (в бинарном представлении)
// 4 бита на олну клетку
// первые 4 бита - первая клетка,
// кажые 4 бита задают число, которое
// лежит в соответствующей клетке

//для удобной свставки и чтения из long
// выбираем нажные 4 бита (квадратик)
const unsigned long long Filter[] = {
    0x000000000000000F, 0x00000000000000F0, 0x0000000000000F00,
    0x000000000000F000, 0x00000000000F0000, 0x0000000000F00000,
    0x000000000F000000, 0x00000000F0000000, 0x0000000F00000000,
    0x000000F000000000, 0x00000F0000000000, 0x0000F00000000000,
    0x000F000000000000, 0x00F0000000000000, 0x0F00000000000000,
    0xF000000000000000
};

const unsigned long long AntiFilter[] = {
    0xFFFFFFFFFFFFFFF0, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFF0FF,
    0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFF0FFFFF,
    0xFFFFFFFFF0FFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFF0FFFFFFFF,
    0xFFFFFF0FFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFF0FFFFFFFFFFF,
    0xFFF0FFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF,
    0x0FFFFFFFFFFFFFFF
};

const unsigned long long RESULT = 1147797409030816545;

struct State {
 public:
  State() {}
  State(const vector<unsigned char> &positions);
  State(const State &state);

  State &operator=(const State &state);

  State up() const;
  State down() const;
  State left() const;
  State right() const;

  bool is_Solved() const;

  void set_Sq(int place, unsigned char value);     // устанавливаем в квадрат place значение value
  unsigned char get_Sq(int place) const;     //поличить значене из квадратика place (1 байт)

  unsigned long long data;    // каком поворотом получен
  int null_pos;       // позиция нуля
};

State::State(const State &state) :
    data(state.data),
    null_pos(state.null_pos) {}

State::State(const vector<unsigned char> &positions) {
  data = 0;
  for (int i = 0; i < 16; i++) {
    set_Sq(i, positions[i]);
    if (positions[i] == 0)
      null_pos = i;
  }
}

bool State::is_Solved() const {
  int invers = 0;
  for (int i = 0; i < 16; i++)
    for (int j = i + 1; j < 16; j++) {
      if (get_Sq(i) != 0 && get_Sq(j) != 0 && get_Sq(j) < get_Sq(i))
        invers++;
    }

  return (invers % 2 == 0) == ((null_pos / 4 ) % 2 == 1);
}

State &State::operator=(const State &state) {
  data = state.data;
  null_pos = state.null_pos;

  return *this;
}

void State::set_Sq(int place, unsigned char value) {
  data = (data & AntiFilter[place]) | (static_cast<unsigned long long>(value) << (place << 2));
  // удалили ст^арую позицию        ^         записали новую и протащили ее а нажное место
}

unsigned char State::get_Sq(int place) const {
  return static_cast<unsigned char>((data & Filter[place]) >> (place << 2));
}

State State::up() const {

  State result(*this);

  unsigned char buff = get_Sq(null_pos - 4);
  result.set_Sq(null_pos - 4, 0);
  result.set_Sq(null_pos, buff);
  result.null_pos -= 4;

  return result;
}

State State::down() const {

  State result(*this);

  unsigned char buff = get_Sq(null_pos + 4);
  result.set_Sq(null_pos + 4, 0);
  result.set_Sq(null_pos, buff);
  result.null_pos += 4;

  return result;
}

State State::left() const {

  State result(*this);

  unsigned char buff = get_Sq(null_pos - 1);
  result.set_Sq(null_pos - 1, 0);
  result.set_Sq(null_pos, buff);
  result.null_pos -= 1;

  return result;
}

State State::right() const {

  State result(*this);

  unsigned char buff = get_Sq(null_pos + 1);
  result.set_Sq(null_pos + 1, 0);
  result.set_Sq(null_pos, buff);
  result.null_pos += 1;

  return result;
}

int Heuristic(const State &state){   // эвристика
  int heuristic = 0;
  for (int i = 0; i < 16; i++) {
    if (state.get_Sq(i) == 0)
      continue;
    heuristic += 10 * (abs(i % 4 - (state.get_Sq(i) - 1) % 4)
        + abs(i / 4 - (state.get_Sq(i) - 1) / 4));
  }
  return heuristic;
}

class AStar {
 public:
  bool operator()(const State &start, list<char> &path) {
    path.clear();

    if (!start.is_Solved())
      return false;

    queue_.emplace(std::make_pair(Heuristic(start), start.data));
    discovered_[start.data] = start;

    parents.emplace(start.data, make_pair(0, 0));

    unsigned long long current;
    while (true) {
      current = queue_.begin()-> second;
      queue_.erase(queue_.begin());

      if (current == RESULT) {
        break;
      }
      State current_state = discovered_[current];

      if (current_state.null_pos >= 4) {  // все возможные варианты
        State next = current_state.up();
        if (discovered_.find(next.data) == discovered_.end()) {  // если ее нет топосещаем
          discovered_[next.data] = next;
          queue_.emplace(std::make_pair(Heuristic(next), next.data));
          parents.emplace(next.data, make_pair('D', current_state.data));
        }
      }
      if (current_state.null_pos <= 11) {
        State next = current_state.down();
        if (discovered_.find(next.data) == discovered_.end()) {  // если ее нет топосещаем
          discovered_[next.data] = next;
          queue_.emplace(std::make_pair(Heuristic(next), next.data));
          parents.emplace(next.data, make_pair('U', current_state.data));
        }
      }
      if (current_state.null_pos % 4 != 0){
        State next = current_state.left();
        if (discovered_.find(next.data) == discovered_.end()) {  // если ее нет топосещаем
          discovered_[next.data] = next;
          queue_.emplace(std::make_pair(Heuristic(next), next.data));
          parents.emplace(next.data, make_pair('R', current_state.data));
        }
      }
      if (current_state.null_pos % 4 != 3){
        State next = current_state.right();
        if (discovered_.find(next.data) == discovered_.end()) {  // если ее нет топосещаем
          discovered_[next.data] = next;
          queue_.emplace(std::make_pair(Heuristic(next), next.data));
          parents.emplace(next.data, make_pair('L', current_state.data));
        }
      }
    }

    unsigned long long curr = RESULT;
    pair<char, unsigned long long> curr_move;
    while(curr != start.data){
      curr_move.first = parents[curr].first;
      curr_move.second =parents[curr].second;
      path.push_front(curr_move.first);
      curr = curr_move.second;
    }
    return true;
  }

 private:


  set<std::pair<int, unsigned long long>> queue_;   //очередь на просмотр
  map<unsigned long long, State> discovered_;    // запоминаем отца и т.д.
  std::unordered_map<unsigned long long, std::pair<char, unsigned long long>> parents;

};

int main() {

  vector<unsigned char> data(16);
  for (int i = 0; i < 16; i++) {
    int x = 0;
    std::cin >> x;
    data[i] = (unsigned char) x;
  }

  State state(data);

  list<char> path;
  AStar solve;
  if (solve(data, path)) {
    std::cout << path.size() << '\n';
    for (auto i:path)
      std::cout << i;
  } else {
    std::cout << -1;
  }

  return 0;

}