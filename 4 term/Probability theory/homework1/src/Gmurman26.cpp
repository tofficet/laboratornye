#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double LENGTH_L = 20.0;
    const double LENGTH_l = 10.0;
    
    // l/L = 10/20 = 0.5
    double theoretical = LENGTH_l / LENGTH_L;
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, LENGTH_L);
        
        int successCount = 0;
        
        double l_start = 5.0;
        double l_end = 15.0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double point = dist(gen);
            if (point >= l_start && point <= l_end) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 26: Отрезок L=20см, меньший отрезок l=10см. Вероятность попадания точки на меньший отрезок", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}