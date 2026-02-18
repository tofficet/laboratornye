#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double a = 10.0; 
    const double r = 5.0;    
 
    double theoretical = (a - r) / a; 
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, 2*a);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double center = dist(gen);
            
            if (center >= r && center <= 2*a - r) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 29: Прямые на расстоянии 2a, монета радиуса r<a. Вероятность не пересечь прямые", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}