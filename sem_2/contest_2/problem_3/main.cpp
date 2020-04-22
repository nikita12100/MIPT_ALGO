#include <fstream>
#include <vector>
#include <assert.h>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <iostream>
#include <string>

using std::vector;
using std::list;
using std::queue;
using std::map;
using std::string;


// храним сетап в ul (в бинарном представлении)
// 4 бита на олну клетку
// первые 4 бита - первая клетка,
// кажые 4 бита задают число, которое
// лежит в соответствующей клетке

//для удобной свставки и чтения из long
// выбираем нажные 4 бита (квадратик)
const unsigned long Filter[] = {
    0x00000000F, 0x0000000F0, 0x000000F00, 0x00000F000, 0x0000F0000,
    0x000F00000, 0x00F000000, 0x0F0000000, 0xF00000000
};

const unsigned long AntiFilter[] = {
    0xFFFFFFFF0, 0xFFFFFFF0F, 0xFFFFFF0FF, 0xFFFFF0FFF, 0xFFFF0FFFF,
    0xFFF0FFFFF, 0xFF0FFFFFF, 0xF0FFFFFFF, 0x0FFFFFFFF
};

const unsigned long RESULT = 2271560481;

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

    unsigned long data;    // каком поворотом получен
    unsigned long father;      // храним отца
    int null_pos;       // позиция нуля
    char move;     // позиция из которой пришли
};


State::State(const State &state) :
        data(state.data),
        null_pos(state.null_pos) {}


State::State(const vector<unsigned char> &positions)
{
    data = 0;
    for (int i = 0; i < 9; i++) {
      set_Sq(i, positions[i]);
        if (positions[i] == 0)
            null_pos = i;
    }
}


bool State::is_Solved() const
{
  int invers = 0;
  for (int i = 0; i < 9; i++)
    for (int j = i + 1; j < 9; j++) {
      if (get_Sq(i) != 0 && get_Sq(j) != 0 && get_Sq(j) < get_Sq(i))
        invers++;
    }

  return invers % 2 == 0;
}


State &State::operator=(const State &state) {
  data = state.data;
  null_pos = state.null_pos;
  father = state.father;
  move = state.move;

  return *this;
}


void State::set_Sq(int place, unsigned char value) {
    data = (data & AntiFilter[place]) | (static_cast<unsigned long>(value) << (place << 2));
    // удалили ст^арую позицию        ^         записали новую и протащили ее а нажное место
}


unsigned char State::get_Sq(int place) const {
    return static_cast<unsigned char>((data & Filter[place]) >> (place << 2));
}


State State::up() const {
    assert(null_pos >= 3);

    State result(*this);

    unsigned char buff = get_Sq(null_pos - 3);
    result.set_Sq(null_pos - 3, 0);
    result.set_Sq(null_pos, buff);
    result.null_pos -= 3;
    result.move = 'U';
    result.father = data;

    return result;
}


State State::down() const
{
    assert(null_pos <= 5);

    State result(*this);

    unsigned char buff = get_Sq(null_pos + 3);
    result.set_Sq(null_pos + 3, 0);
    result.set_Sq(null_pos, buff);
    result.null_pos += 3;
    result.move = 'D';
    result.father = data;

    return result;
}


State State::left() const
{
    assert(null_pos % 3 != 0);

    State result(*this);

    unsigned char buff = get_Sq(null_pos - 1);
    result.set_Sq(null_pos - 1, 0);
    result.set_Sq(null_pos, buff);
    result.null_pos -= 1;
    result.move = 'L';
    result.father = data;

    return result;
}


State State::right() const
{
    assert(null_pos % 3 != 2);

    State result(*this);

    unsigned char buff = get_Sq(null_pos + 1);
    result.set_Sq(null_pos + 1, 0);
    result.set_Sq(null_pos, buff);
    result.null_pos += 1;
    result.move = 'R';
    result.father = data;

    return result;
}


class BFS {
 public:
  bool operator()(const State &start, list<char> &path) {
    path.clear();

    if (!start.is_Solved())
      return false;

    queue_.emplace(start.data);
    discovered_[start.data] = start;

    unsigned long current;
    while (true) {
      current = queue_.front();
      queue_.pop();
      if (current == RESULT) {
        break;
      }
      State current_state = discovered_[current];

      if (current_state.null_pos >= 3)   // все возможные варианты
        visit(current_state.up());
      if (current_state.null_pos <= 5)
        visit(current_state.down());
      if (current_state.null_pos % 3 != 0)
        visit(current_state.left());
      if (current_state.null_pos % 3 != 2)
        visit(current_state.right());
    }
    while (current != start.data) {     // разворачиваем путь от рез до начала
      path.push_front(discovered_[current].move);
      current = discovered_[current].father;
    }
    return true;
  }

 private:
  void visit(const State &state) {
    if (discovered_.find(state.data) == discovered_.end()) {  // если ее нет топосещаем
      discovered_[state.data] = state;
      queue_.emplace(state.data);
    }
  }

  queue<unsigned long> queue_;   //очередь на просмотр
  map<unsigned long, State> discovered_;    // запоминаем отца и т.д.
};


int main() {

    vector<unsigned char> data(9);
    for (int i = 0; i < 9; i++) {
      int x = 0;
      std::cin >> x;
      data[i] = (unsigned char) x;
    }

    State state(data);

     list<char> path;
     BFS solve;
     if(solve(data, path)) {
       std::cout << path.size() << '\n';
       for (auto i:path)
         std::cout << i;
     }
     else{
       std::cout << -1;
     }


  return 0;

}