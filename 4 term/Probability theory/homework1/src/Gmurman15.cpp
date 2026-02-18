#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int TOTAL_ELEMENTS = 5;
    const int WORN_ELEMENTS = 2;
    const int GOOD_ELEMENTS = TOTAL_ELEMENTS - WORN_ELEMENTS;
    const int CHOSEN_ELEMENTS = 2;
    
    // Теоретическая вероятность: C(3,2)/C(5,2)
    double theoretical = combination(GOOD_ELEMENTS, CHOSEN_ELEMENTS) / combination(TOTAL_ELEMENTS, CHOSEN_ELEMENTS);
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> elements(TOTAL_ELEMENTS);
            for (int i = 0; i < TOTAL_ELEMENTS; i++) {
                elements[i] = (i < GOOD_ELEMENTS) ? 1 : 0;
            }

            std::shuffle(elements.begin(), elements.end(), gen);

            bool allGood = true;
            for (int i = 0; i < CHOSEN_ELEMENTS; i++) {
                if (elements[i] == 0) {
                    allGood = false;
                    break;
                }
            }
            
            if (allGood) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 15: Устройство из 5 элементов (2 изношены). Вероятность включить 2 хороших", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}