#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

double theoretical() {
    return 57.0 / 115.0;
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
            std::vector<int> questions(25);
            for (int i = 0; i < 25; i++) {
                questions[i] = (i < 20) ? 1 : 0;
            }
            
            std::shuffle(questions.begin(), questions.end(), gen);
            
            bool allKnown = true;
            for (int i = 0; i < 3; i++) {
                if (questions[i] == 0) {
                    allKnown = false;
                    break;
                }
            }
            
            if (allKnown) {
                successCount++;
            }
        }
        
        empirical.push_back(static_cast<double>(successCount) / numTrials);
    }
    
    printResults("ЗАДАЧА 69: Студент знает 20 из 25 вопросов. Вероятность, что знает все 3 предложенных вопроса", 
                 THEOR, trialCounts, empirical);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "Всего способов выбрать 3 вопроса из 25: C(25,3) = 2300\n";
    std::cout << "Способов выбрать 3 вопроса из 20 известных: C(20,3) = 1140\n";
    std::cout << "P = 1140/2300 = 57/115 = " << THEOR << std::endl;
    
    return 0;
}