#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

double theoretical() {
    return 1.0 / 495.0;
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
            std::vector<int> tickets(100);
            for (int i = 0; i < 100; i++) {
                tickets[i] = (i < 5) ? 1 : 0;
            }
            
            std::shuffle(tickets.begin(), tickets.end(), gen);
            
            if (tickets[0] == 1 && tickets[1] == 1) {
                successCount++;
            }
        }
        
        empirical.push_back(static_cast<double>(successCount) / numTrials);
    }
    
    printResults("ЗАДАЧА 65: Среди 100 билетов 5 выигрышных. Вероятность, что 2 наудачу выбранных билета выигрышные", 
                 THEOR, trialCounts, empirical);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "Всего способов выбрать 2 билета из 100: C(100,2) = 4950\n";
    std::cout << "Способов выбрать 2 выигрышных из 5: C(5,2) = 10\n";
    std::cout << "P = 10/4950 = 1/495 = " << THEOR << std::endl;
    
    return 0;
}