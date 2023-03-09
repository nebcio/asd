#include <iostream>
#include <vector>

// z5 - horner

double oblicz(std::vector<double> a, double x) {
    // a0 + x(a1 + x(a2 + x(... + x(a[n-1] + x(a[n]))...)))
    double wynik = x * a[a.size() - 1];
    for (int i = a.size() - 2; i > 0; --i) {
        wynik = x * (wynik + a[i]);
    }
    return wynik + a[0];
}

int main() {
    std::cout << oblicz({4, 5, 3, 0, 0, 18}, 10) << std::endl;
}