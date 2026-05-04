#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>

int theoreticalMinStudents() {
    int n = 1;
    while (true) {
        double prob = 1.0 - 0.7 * std::pow(0.5, n);
        if (prob > 0.9) {
            return n;
        }
        n++;
    }
}

void printResults(const std::string& title, 
                  int theoreticalN,
                  const std::vector<int>& trialCounts,
                  const std::vector<int>& empiricalN) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретически минимальное количество студентов (помимо старосты) = " << theoreticalN << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Серий испытаний: " << trialCounts[i] 
                  << " | эмпирически найденное n = " << empiricalN[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int THEOR_N = theoreticalMinStudents();
    
    std::vector<int> empiricalN;
    
    for (int numSeries : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        int totalN = 0;
        
        for (int series = 0; series < numSeries; series++) {
            int n = 1;
            while (true) {
                bool informed = false;
                
                if (dis(gen) <= 0.3) {
                    informed = true;
                } else {
                    for (int student = 0; student < n; student++) {
                        if (dis(gen) <= 0.5) {
                            informed = true;
                            break;
                        }
                    }
                }
                
                double theoreticalProb = 1.0 - 0.7 * std::pow(0.5, n);
                if (informed && theoreticalProb > 0.9) {
                    totalN += n;
                    break;
                }
                n++;
            }
        }
        
        empiricalN.push_back(totalN / numSeries);
    }
    
    printResults("ЗАДАЧА 68: Письмо старосте и n студентам. Вероятность оповестить: староста 0.3, студент 0.5. Найти n, чтобы вероятность информирования > 0.9", 
                 THEOR_N, trialCounts, empiricalN);
    
    return 0;
}