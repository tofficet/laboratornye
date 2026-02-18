#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double R_big = 10.0;
    const double R_small = 5.0;
    
    // Теоретическая вероятность: (R_big^2 - R_small^2) / R_big^2
    double theoretical = (R_big * R_big - R_small * R_small) / (R_big * R_big); 
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double angle = dist(gen) * 2.0 * M_PI;
            double radius = R_big * sqrt(dist(gen));
            
            if (radius >= R_small) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 32: Концентрические окружности R=10см и r=5см. Вероятность попадания в кольцо", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}