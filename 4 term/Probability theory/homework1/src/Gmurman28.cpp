#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double R = 10.0;
    const double r = 5.0;
    
    // Теоретическая вероятность: (r^2)/(R^2)
    double theoretical = (r * r) / (R * R); 
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(-R, R);
        
        int successCount = 0;
        int attempts = 0;
        
        while (attempts < numTrials) {
            double x = dist(gen);
            double y = dist(gen);
         
            double dist_to_center = sqrt(x*x + y*y);
            if (dist_to_center <= R) {
                attempts++;
            
                bool hits = (dist_to_center <= r);
                
                if (hits) {
                    successCount++;
                }
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 28: Круг радиуса R=10, внутри круг радиуса r=5. Вероятность попадания в малый круг", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}