#include <iostream>
#include "Array.h"


int main()
{
    Array<int> a(8);
    for (int i = 0; i < a.size(); i++)
        a[i] = i;

    std::cout << a << std::endl;

    Array<int> b = a << 2;
    std::cout << b << std::endl;

    b >> 3;
    std::cout << b << std::endl;

    b.reverse().reverse(2, 5);
    std::cout << b << std::endl;

    b.remove(9);
    std::cout << b << std::endl;

    return 0;
}
