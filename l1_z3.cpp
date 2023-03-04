#include <iostream>

//z3a - x^n  [log2 n] recursive
int power_rec(int x, int n) {
    // std::cout << "x: " << x << " n: " << n << std::endl;
    if (n == 0) return 1;
    if (n % 2 == 0) return power_rec(x * x, n / 2); // /2 and x*x -> log2 n
    return x * power_rec(x * x, (n - 1) / 2);
}

//z3b - x^n  [log2 n] iterative
int power2_it(int x, int n) {
    int result = 1;
    while (n > 0) {
        std::cout << "x:\t" << x << "\tn:\t" << n << "\tresult:\t" << result << std::endl;
        if (n % 2 == 1) result *= x;    // if n is odd then multiply result by x
        std::cout << "x:\t" << x << "\tn:\t" << n << "\tresult:\t" << result << std::endl;

        x *= x; // x^2  
        n /= 2; // /2 -> log2 n; 1/2 -> 0
    }
    return result;
}

int main() {
    // z3a - x^n  [log2 n] recursive
    std::cout << power_rec(3, 10) << std::endl;
    // z3b - x^n  [log2 n] iterative
    std::cout << power2_it(3, 16) << std::endl;
}

 