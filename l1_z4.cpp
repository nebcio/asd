#include <iostream>
#include <cmath>

// z4 root with bisect
double root_bisect(double a, double b, double f(double), double epsilon = 0.0001) {    
    // a, b - interval, a < b, f - continuous function, epislon - precision

    // if f(a)*f(b) > 0 then there is no root in [a, b]
    if (f(a) * f(b) > 0) {
        std::cout << "No root in interval [" << a << ", " << b << "]" << std::endl;
        return -1.0;   // no root in [a, b]
    }


    double c = 0.0;                    
    // if f(a)*f(b) < 0 then there is a root in [a, b]
    // if f(a)*f(b) == 0 then a or b is a root (end of loop)
    while (f(a) * f(b) < 0) {
        c = (a + b) / 2;                    // middle of [a, b] 
        std::cout << "a:\t" << a << "\tb:\t" << b << "\tc:\t" << c << std::endl;
        if (f(a) * f(c) < 0) b = c;         // root in [a, c]
        else a = c;                         // root in [c, b]
        if (fabs(a - b) < epsilon) return c;      // a == b, root
    }
}

int main() {
    // z4 root with bisect
    std::cout << root_bisect(0, 1, [](double x) { return x*x - 0.5; }) << std::endl;
    
}