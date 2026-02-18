#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double a = 10.0;
    const double r = 3.0;     
    // Теоретическая вероятность: ((a - 2r)/a)^2
    double theoretical = (a - 2*r) / a;
    theoretical = theoretical * theoretical; 
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, a);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double x = dist(gen);
            double y = dist(gen);
            
            if (x >= r && x <= a - r && y >= r && y <= a - r) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 30: Сетка квадратов со стороной a, монета радиуса r. Вероятность не пересечь линии", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}