#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int NUM_CUBES = 6;
    double theoretical = 1.0 / factorial(NUM_CUBES);
    
    std::vector<double> empirical;
    for (int numTrials : trialCounts) {
        
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> cubes(NUM_CUBES);
            for (int i = 0; i < NUM_CUBES; i++) {
                cubes[i] = i + 1;
            }
            
            std::shuffle(cubes.begin(), cubes.end(), gen);
            
            bool isIncreasing = true;
            for (int i = 1; i < NUM_CUBES; i++) {
                if (cubes[i] < cubes[i-1]) {
                    isIncreasing = false;
                    break;
                }
            }
            
            if (isIncreasing) {
                successCount++;
            }
            
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 8: Извлечение 6 пронумерованных кубиков в возрастающем порядке", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}