#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    double theoretical = 0.0;
    
  
    
    //  x^2 - x + 0.09 = 0
    double discriminant = 1.0 - 4.0 * 0.09;  
    double x_min = (1.0 - sqrt(discriminant)) / 2.0;  
    double x_max = (1.0 + sqrt(discriminant)) / 2.0;  
    
    // Интегрируем (1 - x - 0.09/x) от x_min до x_max
    theoretical = (x_max - x_min) - (x_max*x_max - x_min*x_min)/2.0 - 0.09 * (log(x_max) - log(x_min));
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double x = dist(gen);
            double y = dist(gen);
            
            if (x + y <= 1.0 && x * y >= 0.09) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 45: Два числа x,y ∈ [0,1]. Вероятность, что x+y ≤ 1 и xy ≥ 0.09", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}