#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>

double factorial(int a) {
    if (a < 0) return 0;
    if (a == 0) return 1;
    
    double f = 1.0;
    for (int i = 2; i <= a; i++) {
        f *= i;
    }
    return f;
}

double combination(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;  

    double result = 1.0;
    for (int i = 1; i <= k; ++i) {
        result *= (n - k + i);
        result /= i;
    }
    return result;
}

void printResults(const std::string& taskName, double theoretical, 
                  const std::vector<int>& trialCounts, 
                  const std::vector<double>& empiricalResults) {
    
    std::cout << "\n" << taskName << "\n";
    std::cout << "Теоретическая вероятность: " << std::fixed << std::setprecision(10) << theoretical << "\n\n";
    std::cout << "ЭМПИРИЧЕСКОЕ МОДЕЛИРОВАНИЕ:\n";
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Количество испытаний: " << trialCounts[i] << std::endl;
        std::cout << "Эмпирическая вероятность: " << std::fixed << std::setprecision(10) << empiricalResults[i]<<"\n";

        int expectedSuccesses = static_cast<int>(trialCounts[i] * theoretical);
        int actualSuccesses = static_cast<int>(trialCounts[i] * empiricalResults[i] + 0.5);
        std::cout << "Ожидаемое число успехов: " << expectedSuccesses;
        std::cout << ", фактическое: " << actualSuccesses << "\n\n";
    }
}


void printResults1(const std::string& title, int N, int numTrials,
                  double theor_a, double theor_b, double theor_c,
                  double emp_a, double emp_b, double emp_c) {
    std::cout << "\n" << title << "\n";
    std::cout << "N = " << N << ", испытаний = " << numTrials << "\n\n";

    std::cout << "а) Вероятность, что первый символ четный" << ":\n";
    std::cout << "   Аналитическая: " << std::fixed << std::setprecision(10) << theor_a << "\n";
    std::cout << "   Эмпирическая:  " << emp_a << "\n";
    std::cout << "   Отклонение:    " << std::abs(emp_a - theor_a) << "\n\n";

    std::cout << "б) Вероятность, что второй символ четный" << ":\n";
    std::cout << "   Аналитическая: " << theor_b << "\n";
    std::cout << "   Эмпирическая:  " << emp_b << "\n";
    std::cout << "   Отклонение:    " << std::abs(emp_b - theor_b) << "\n\n";

    std::cout << "в) Вероятность, что оба символа четные" << ":\n";
    std::cout << "   Аналитическая: " << theor_c << "\n";
    std::cout << "   Эмпирическая:  " << emp_c << "\n";
    std::cout << "   Отклонение:    " << std::abs(emp_c - theor_c) << "\n";
}


void print_results3(int i, int j,
                   const std::vector<std::vector<int>>& union_perms,
                   const std::vector<std::vector<int>>& inter_perms,
                   double count_i, double count_j,
                   double count_ij, double count_union,
                   double total) {
    std::cout << "\nПерестановки, удовлетворяющие A_" << i << " ∪ A_" << j
              << " =" << union_perms.size() << "\n";

    std::cout << "\nПерестановки, удовлетворяющие A_" << i << " ∩ A_" << j
              << " = " << inter_perms.size() << "\n";

    double p_i = count_i / total;
    double p_j = count_j / total;
    double p_ij = count_ij / total;
    double p_union = count_union / total;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Всего перестановок: " << total << "\n";
    std::cout << "A_" << i << ": число = " << count_i << ", вероятность = " << p_i << "\n";
    std::cout << "A_" << j << ": число = " << count_j << ", вероятность = " << p_j << "\n";
    std::cout << "A_" << i << " * A_" << j << ": число = " << count_ij << ", вероятность = " << p_ij << "\n";
    std::cout << "A_" << i << " + A_" << j << ": число = " << count_union << ", вероятность = " << p_union << "\n";
}

#endif