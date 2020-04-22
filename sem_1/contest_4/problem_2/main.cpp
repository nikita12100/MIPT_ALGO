/*
В город N приехал цирк с командой атлетов.
Они хотят удивить горожан города N — выстроить из своих тел башню максимальной высоты.
Башня — это цепочка атлетов, первый стоит на земле, второй стоит у него на плечах, третий
стоит на плечах у второго и т.д. Каждый атлет характеризуется силой si (kg) и массой mi (kg).
Сила — это максимальная масса, которую атлет способен держать у себя на плечах.
К сожалению ни один из атлетов не умеет программировать,
так как всю жизнь они занимались физической подготовкой, и у них не было времени на изучение
языков программирования. Помогите им, напишите программу, которая определит максимальную
высоту башни, которую они могут составить. Известно, что если атлет тяжелее, то он и сильнее:
если mi>mj, то si > sj.
Атлеты равной массы могут иметь различную силу.
*/

#include <iostream>
#include <list>

using namespace std;
//////////////////// compare <
const bool Atlet_comare(pair<int,int>& p1, pair<int,int>& p2){
  if(p1.first < p2.first) return true;
  else if(p1.first == p2.first)
    if (p1.second < p2.second) return true;
  return false;
}

int Height(list<pair<int, int>> & atlets){
  int weight = 0;
  int height = 0;
  for(auto i:atlets){
    if(i.second >= weight){
      weight += i.first;
      height++;
    }
  }
  return height;
}


int main(){
  list<pair<int, int>> atlets; // масса, сила

  int m_buff,s_buff = 0;
  while(cin >> m_buff >> s_buff){
    atlets.push_back(pair<int,int>(m_buff, s_buff));
  }

  atlets.sort(Atlet_comare);

  cout << Height(atlets);

  return 0;
}