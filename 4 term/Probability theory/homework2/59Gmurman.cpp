#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalTwoOnes() {
    return 15.0 / 216.0;
}

double theoreticalTwoSame() {
    return 90.0 / 216.0;
}

double theoreticalAllDifferent() {
    return 120.0 / 216.0;
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB, double theoreticalC,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB,
                  const std::vector<double>& empiricalC) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) на двух гранях 1, на третьей другое = " << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) на двух гранях одинаковое, на третьей другое = " << theoreticalB << std::endl;
    std::cout << "Теоретическая вероятность в) на всех гранях разное = " << theoreticalC << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::fixed << std::setprecision(6) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i]
                  << " | в) эмпир. = " << empiricalC[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR_A = theoreticalTwoOnes();
    const double THEOR_B = theoreticalTwoSame();
    const double THEOR_C = theoreticalAllDifferent();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    std::vector<double> empiricalC;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(1, 6);
        
        int successA = 0;
        int successB = 0;
        int successC = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            int dice1 = dis(gen);
            int dice2 = dis(gen);
            int dice3 = dis(gen);
            
            int countOnes = (dice1 == 1) + (dice2 == 1) + (dice3 == 1);
            if (countOnes == 2) {
                successA++;
            }
            
            if ((dice1 == dice2 && dice2 != dice3) ||
                (dice1 == dice3 && dice3 != dice2) ||
                (dice2 == dice3 && dice3 != dice1)) {
                successB++;
            }
            
            if (dice1 != dice2 && dice1 != dice3 && dice2 != dice3) {
                successC++;
            }
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
        empiricalC.push_back(static_cast<double>(successC) / numTrials);
    }
    
    printResults("ЗАДАЧА 59: Брошены три игральные кости", 
                 THEOR_A, THEOR_B, THEOR_C, trialCounts, empiricalA, empiricalB, empiricalC);
    
    return 0;
}