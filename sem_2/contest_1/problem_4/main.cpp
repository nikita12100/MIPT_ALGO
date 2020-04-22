#include <iostream>
#include <vector>

bool Is_stock(const std::vector<std::vector<int>> & matrix){
  unsigned int n = matrix.size();
  std::vector<bool> stock(n, true);
  for (int i = 0; i < n; ++i) {
      if(!stock[i])
        continue;
     if(matrix[i][i] == 0){
       int line = (i + 1) % n;
       while (line != i){ //проверяем линию
         if(matrix[i][line] == 0)   //в столбе есть 0
           stock[line] = false;
         else{
           stock[i] = false;
           break;
         }
         line = (line + 1) % n;
       }

       if(line == i){ // если в гориз. нет 1 проверяем столбец
         for (int j = 0; j < n; ++j) {
           if(matrix[j][i] == 0 && j != i) // в столбце нет 0, это сток
             return false;
         }
         return true;      // в гор. нет 1 и в столб. нет 0, это сток
       }
     }
    else{
       stock[i] = false;      // есть выходящая
     }
  }
  return false;
}


int main(){
  int n = 0;
  std::cin >> n;

  std::vector<std::vector<int>> matrix(n);
  for (int i = 0; i < n; ++i)
    matrix[i].resize(n);

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      std::cin >> matrix[i][j];


  if(Is_stock(matrix))
    std::cout << "YES";
  else
    std::cout << "NO";

  return 0;
}