#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int TOTAL_KINESCOPES = 15;
    const int LVIV_KINESCOPES = 10;
    const int OTHER_KINESCOPES = TOTAL_KINESCOPES - LVIV_KINESCOPES; 
    const int CHOSEN_KINESCOPES = 5;
    const int DESIRED_LVIV = 3;
    const int DESIRED_OTHER = CHOSEN_KINESCOPES - DESIRED_LVIV;
    
    // Теоретическая вероятность: C(10,3) * C(5,2) / C(15,5)
    double theoretical = (combination(LVIV_KINESCOPES, DESIRED_LVIV) * 
                          combination(OTHER_KINESCOPES, DESIRED_OTHER)) / 
                          combination(TOTAL_KINESCOPES, CHOSEN_KINESCOPES);
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> kinescopes(TOTAL_KINESCOPES);
            for (int i = 0; i < TOTAL_KINESCOPES; i++) {
                kinescopes[i] = (i < LVIV_KINESCOPES) ? 1 : 0;
            }
            
            std::shuffle(kinescopes.begin(), kinescopes.end(), gen);
            
            int lvivCount = 0;
            for (int i = 0; i < CHOSEN_KINESCOPES; i++) {
                if (kinescopes[i] == 1) {
                    lvivCount++;
                }
            }
            
            if (lvivCount == DESIRED_LVIV) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 19: 15 кинескопов (10 Львовских). Вероятность, что среди 5 взятых ровно 3 Львовских", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}