#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double L = 1.0;
    
    // Теоретическая вероятность
    // Меньший из отрезков OB и BA > L/3
    // Это выполняется, когда x ∈ (L/3, 2L/3)
    // Длина этого интервала = L/3
    double theoretical = (L/3) / L; 
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, L);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double x = dist(gen);
            
            if (x > L/3.0 && x < 2.0*L/3.0) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 27: На отрезке [0,L] случайная точка B. Вероятность, что меньший из OB и BA > L/3", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}