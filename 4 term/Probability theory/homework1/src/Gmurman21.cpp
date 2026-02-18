#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int TOTAL_ITEMS = 5;
    const int PAINTED_ITEMS = 3;
    const int UNPAINTED_ITEMS = TOTAL_ITEMS - PAINTED_ITEMS;
    const int CHOSEN_ITEMS = 2;
    
    // а) ровно одно окрашенное: C(3,1) * C(2,1) / C(5,2)
    double theoretical_one_painted = (combination(PAINTED_ITEMS, 1) * combination(UNPAINTED_ITEMS, 1)) / 
                                      combination(TOTAL_ITEMS, CHOSEN_ITEMS);
    
    // б) два окрашенных: C(3,2) * C(2,0) / C(5,2)
    double theoretical_two_painted = (combination(PAINTED_ITEMS, 2) * combination(UNPAINTED_ITEMS, 0)) / 
                                      combination(TOTAL_ITEMS, CHOSEN_ITEMS);
    
    // в) хотя бы одно окрашенное = 1 - ни одного окрашенного
    double theoretical_none_painted = combination(UNPAINTED_ITEMS, 2) / combination(TOTAL_ITEMS, CHOSEN_ITEMS);
    double theoretical_at_least_one = 1.0 - theoretical_none_painted;
    
    std::vector<double> empirical_one_painted;
    std::vector<double> empirical_two_painted;
    std::vector<double> empirical_none_painted;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int count_one_painted = 0;
        int count_two_painted = 0;
        int count_none_painted = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> items(TOTAL_ITEMS);
            for (int i = 0; i < TOTAL_ITEMS; i++) {
                items[i] = (i < PAINTED_ITEMS) ? 1 : 0;
            }

            std::shuffle(items.begin(), items.end(), gen);
            
            int paintedCount = 0;
            for (int i = 0; i < CHOSEN_ITEMS; i++) {
                if (items[i] == 1) {
                    paintedCount++;
                }
            }
            
            if (paintedCount == 1) {
                count_one_painted++;
            } else if (paintedCount == 2) {
                count_two_painted++;
            } else { 
                count_none_painted++;
            }
        }
        
        empirical_one_painted.push_back(static_cast<double>(count_one_painted) / numTrials);
        empirical_two_painted.push_back(static_cast<double>(count_two_painted) / numTrials);
        empirical_none_painted.push_back(static_cast<double>(count_none_painted) / numTrials);
    }
    

    printResults("ЗАДАЧА 21 (а): 5 изделий (3 окрашенных). Вероятность, что среди 2 извлеченных ровно 1 окрашенное", 
                 theoretical_one_painted, trialCounts, empirical_one_painted);
    
    printResults("(б): 5 изделий (3 окрашенных). Вероятность, что среди 2 извлеченных оба окрашенные", 
                 theoretical_two_painted, trialCounts, empirical_two_painted);
    
    std::vector<double> empirical_at_least_one;
    for (size_t i = 0; i < empirical_none_painted.size(); i++) {
        empirical_at_least_one.push_back(1.0 - empirical_none_painted[i]);
    }
    
    printResults("ЗАДАЧА 21 (в): 5 изделий (3 окрашенных). Вероятность, что среди 2 извлеченных хотя бы одно окрашенное (1 - P(ни одного))", 
                 theoretical_at_least_one, trialCounts, empirical_at_least_one);
    
    return 0;
}