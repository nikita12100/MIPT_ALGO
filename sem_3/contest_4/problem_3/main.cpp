#include <iostream>
#include <set>

int main(){
    std::set<int> numbers;
    numbers.emplace(1);
    numbers.emplace(2);
    numbers.emplace(3);

    int res_cost = 0;
    while(!numbers.empty()){
        int first = *numbers.begin();
        numbers.erase(numbers.begin());
        int second = *numbers.begin();
        numbers.erase(numbers.begin());

        int curr_cost = first*second;
        res_cost += curr_cost;

        numbers.emplace(curr_cost);
    }

    std::cout << res_cost;
    return 0;
}

