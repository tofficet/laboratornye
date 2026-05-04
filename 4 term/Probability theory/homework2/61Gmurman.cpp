#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>

double theoreticalNoMiss(int n) {
    return std::pow(0.8, n);
}

int findMinShots() {
    int n = 1;
    while (std::pow(0.8, n) >= 0.4) {
        n++;
    }
    return n;
}

void printResults(const std::string& title, 
                  int theoreticalN,
                  const std::vector<int>& trialCounts,
                  const std::vector<int>& empiricalN) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретически минимальное количество выстрелов = " << theoreticalN << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Серий испытаний: " << trialCounts[i] 
                  << " | эмпирически найденное n = " << empiricalN[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int THEOR_N = findMinShots();
    
    std::vector<int> empiricalN;
    
    for (int numSeries : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        int totalN = 0;
        
        for (int series = 0; series < numSeries; series++) {
            int n = 1;
            while (true) {
                bool allHit = true;
                for (int shot = 0; shot < n; shot++) {
                    if (dis(gen) > 0.8) {
                        allHit = false;
                        break;
                    }
                }
                if (allHit) {
                    if (std::pow(0.8, n) < 0.4) {
                        totalN += n;
                        break;
                    }
                }
                n++;
            }
        }
        
        empiricalN.push_back(totalN / numSeries);
    }
    
    printResults("ЗАДАЧА 61: Вероятность попадания 0.8. Сколько выстрелов, чтобы вероятность ни одного промаха была < 0.4", 
                 THEOR_N, trialCounts, empiricalN);
    
    std::cout << "\nПроверка:\n";
    std::cout << "n = 4: 0.8^4 = " << std::pow(0.8, 4) << " >= 0.4\n";
    std::cout << "n = 5: 0.8^5 = " << std::pow(0.8, 5) << " < 0.4\n";
    
    return 0;
}