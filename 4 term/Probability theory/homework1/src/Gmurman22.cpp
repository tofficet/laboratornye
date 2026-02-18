#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int DISKS = 4;
    const int SECTORS_PER_DISK = 5;
    
    // Теоретическая вероятность: 1 / (5^4)
    double theoretical = 1.0 / std::pow(SECTORS_PER_DISK, DISKS);
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dist(0, SECTORS_PER_DISK - 1);
        
        int successCount = 0;
        
        std::vector<int> correctCombination = {1, 2, 3, 4};
        
        for (int trial = 0; trial < numTrials; trial++) {
            bool isOpen = true;
            for (int i = 0; i < DISKS; i++) {
                if (dist(gen) + 1 != correctCombination[i]) {
                    isOpen = false;
                    break;
                }
            }
            
            if (isOpen) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 22: Секретный замок с 4 дисками по 5 цифр. Вероятность открыть при случайной установке", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}