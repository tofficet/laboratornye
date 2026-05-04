#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

double theoretical() {
    return 1.0 / 14.0;
}

void printResults(const std::string& title, 
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность = " << std::fixed << std::setprecision(8) << theoretical << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | эмпир. = " << std::setprecision(8) << empirical[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR = theoretical();
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> details(10);
            for (int i = 0; i < 10; i++) {
                details[i] = (i < 6) ? 1 : 0;
            }
            
            std::shuffle(details.begin(), details.end(), gen);
            
            bool allColored = true;
            for (int i = 0; i < 4; i++) {
                if (details[i] == 0) {
                    allColored = false;
                    break;
                }
            }
            
            if (allColored) {
                successCount++;
            }
        }
        
        empirical.push_back(static_cast<double>(successCount) / numTrials);
    }
    
    printResults("ЗАДАЧА 67: В ящике 10 деталей, среди которых 6 окрашенных. Наудачу извлекают 4 детали. Вероятность, что все окрашенные", 
                 THEOR, trialCounts, empirical);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "Всего способов выбрать 4 детали из 10: C(10,4) = 210\n";
    std::cout << "Способов выбрать 4 окрашенных из 6: C(6,4) = 15\n";
    std::cout << "P = 15/210 = 1/14 = " << THEOR << std::endl;
    
    return 0;
}