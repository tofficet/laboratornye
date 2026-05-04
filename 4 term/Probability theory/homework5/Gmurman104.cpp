#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalB3() {
    return 1.0 - 0.6 - 0.3;
}

int main() {
    const double THEOR = theoreticalB3();

    std::cout << "\nЗАДАЧА 104: Условная вероятность гипотезы B3" << std::endl;
    std::cout << "P_A(B1) = 0.6" << std::endl;
    std::cout << "P_A(B2) = 0.3" << std::endl;
    std::cout << "P_A(B3) = 1 - 0.6 - 0.3 = "
              << std::fixed << std::setprecision(1) << THEOR << std::endl;

    return 0;
}