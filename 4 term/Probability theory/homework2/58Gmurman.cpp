#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalAllFives() {
    return 1.0 / 216.0;
}

double theoreticalAllSame() {
    return 6.0 / 216.0;
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) на всех гранях пять очков = " << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) на всех гранях одинаковое число = " << theoreticalB << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::fixed << std::setprecision(6) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR_A = theoreticalAllFives();
    const double THEOR_B = theoreticalAllSame();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(1, 6);
        
        int successA = 0;
        int successB = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            int dice1 = dis(gen);
            int dice2 = dis(gen);
            int dice3 = dis(gen);
            
            if (dice1 == 5 && dice2 == 5 && dice3 == 5) {
                successA++;
            }
            
            if (dice1 == dice2 && dice2 == dice3) {
                successB++;
            }
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
    }
    
    printResults("ЗАДАЧА 58: Брошены три игральные кости", 
                 THEOR_A, THEOR_B, trialCounts, empiricalA, empiricalB);
    
    return 0;
}