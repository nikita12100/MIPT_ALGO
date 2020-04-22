#include <iostream>
#include <cmath>

using namespace std;

int NextPrimeNumber(int previous)
{
    int temp = previous + 1;
    int next = 0;
    int exit = 0;
    int i = 2;

    while (exit == 0)
    {
        if (temp%i == 0 && i!=temp)
        {
            temp++;
            i=2;
            continue;
        }

        if ((i==temp) || (i>sqrt(temp)))
        {
            next = temp;
            exit = 1;
        }

        i++;
    }

    return next;
}

int main() {
    int n = 0;
    cin >> n;

    for (int i = 2; i < 15; ++i) {
        cout << NextPrimeNumber(i) << " ";
    }
    return 0;
}