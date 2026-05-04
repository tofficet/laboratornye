#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

double theoreticalWithoutReturn() {
    return 1.0 / 720.0;
}

double theoreticalWithReturn() {
    return 1.0 / 1000.0;
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) без возвращения = " << std::fixed << std::setprecision(8) << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) с возвращением = " << theoreticalB << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::setprecision(8) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR_A = theoreticalWithoutReturn();
    const double THEOR_B = theoreticalWithReturn();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(1, 10);
        
        int successA = 0;
        int successB = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> cubes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::shuffle(cubes.begin(), cubes.end(), gen);
            
            if (cubes[0] == 1 && cubes[1] == 2 && cubes[2] == 3) {
                successA++;
            }
            
            int first = dis(gen);
            int second = dis(gen);
            int third = dis(gen);
            
            if (first == 1 && second == 2 && third == 3) {
                successB++;
            }
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
    }
    
    printResults("ЗАДАЧА 70: В мешочке кубики с номерами 1-10. Извлекают 3 кубика. Вероятность, что появятся 1,2,3 последовательно", 
                 THEOR_A, THEOR_B, trialCounts, empiricalA, empiricalB);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "а) Без возвращения:\n";
    std::cout << "   P = 1/10 * 1/9 * 1/8 = 1/720 = " << THEOR_A << std::endl;
    std::cout << "б) С возвращением:\n";
    std::cout << "   P = (1/10)^3 = 1/1000 = " << THEOR_B << std::endl;
    
    return 0;
}