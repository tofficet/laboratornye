#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalAtMostThree() {
    double allFour = 0.6 * 0.7 * 0.8 * 0.9;
    return 1.0 - allFour;
}

double theoreticalAtLeastTwo() {
    double p0 = 0.4 * 0.3 * 0.2 * 0.1;
    double p1 = 0.6*0.3*0.2*0.1 + 0.7*0.4*0.2*0.1 + 0.8*0.4*0.3*0.1 + 0.9*0.4*0.3*0.2;
    return 1.0 - (p0 + p1);
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) P(не более чем в трех) = " << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) P(не менее чем в двух) = " << theoreticalB << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::fixed << std::setprecision(6) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR_A = theoreticalAtMostThree();
    const double THEOR_B = theoreticalAtLeastTwo();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        int successA = 0;
        int successB = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            int countInBoxes = 0;
            if (dis(gen) <= 0.6) countInBoxes++;
            if (dis(gen) <= 0.7) countInBoxes++;
            if (dis(gen) <= 0.8) countInBoxes++;
            if (dis(gen) <= 0.9) countInBoxes++;
            
            if (countInBoxes <= 3) successA++;
            if (countInBoxes >= 2) successB++;
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
    }
    
    printResults("ЗАДАЧА 57: Вероятности 0.6, 0.7, 0.8, 0.9", 
                 THEOR_A, THEOR_B, trialCounts, empiricalA, empiricalB);
    
    return 0;
}