#include <iostream>

#define MAX_DANGEROUS(a , b) (a > b ? a : b)

inline int max( int a, int b ) {
    return a > b ? a : b;
}

int main () {
    int a = 1, b = 2, result = 0;
    
    std::cout << "Before: a: " << a << " b: " << b << std::endl;
    result = max ( a++, b++ );
    std::cout << "max: " << result << std::endl;
    std::cout << "After:  a: " << a << " b: " << b << std::endl;

    a = 1;
    b = 2;

    // undefined behaviour with b++
    std::cout << std::endl;
    std::cout << "Before: a: " << a << " b: " << b << std::endl;
    result = MAX_DANGEROUS( a++, b++ );
    std::cout << "max: " << result << std::endl;
    std::cout << "After:  a: " << a << " b: " << b << std::endl;

    return 0;
}
