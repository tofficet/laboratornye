#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

double theoreticalSequential() {
    return 1.0 / 60.0;
}

double theoreticalAnyOrder() {
    return 1.0 / 10.0;
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) последовательно 1,4,5 = " << std::fixed << std::setprecision(8) << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) номера 1,4,5 в любом порядке = " << theoreticalB << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::setprecision(8) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR_A = theoreticalSequential();
    const double THEOR_B = theoreticalAnyOrder();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successA = 0;
        int successB = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> balls = {1, 2, 3, 4, 5};
            std::shuffle(balls.begin(), balls.end(), gen);
            
            if (balls[0] == 1 && balls[1] == 4 && balls[2] == 5) {
                successA++;
            }
            
            std::vector<int> firstThree = {balls[0], balls[1], balls[2]};
            std::sort(firstThree.begin(), firstThree.end());
            if (firstThree[0] == 1 && firstThree[1] == 4 && firstThree[2] == 5) {
                successB++;
            }
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
    }
    
    printResults("ЗАДАЧА 68: В урне шары с номерами 1-5. Извлекают 3 шара без возвращения", 
                 THEOR_A, THEOR_B, trialCounts, empiricalA, empiricalB);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "Всего упорядоченных исходов: 5*4*3 = 60\n";
    std::cout << "а) Благоприятных исходов (1,4,5 в точном порядке): 1\n";
    std::cout << "   P = 1/60 = " << THEOR_A << std::endl;
    std::cout << "б) Благоприятных исходов (1,4,5 в любом порядке): 3! = 6\n";
    std::cout << "   P = 6/60 = 1/10 = " << THEOR_B << std::endl;
    
    return 0;
}