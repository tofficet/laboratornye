#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int TOTAL_CARDS = 100;
    const int DRAWN_CARDS = 10;
    
    double theoretical = 1.0 - combination(TOTAL_CARDS - 1, DRAWN_CARDS) / combination(TOTAL_CARDS, DRAWN_CARDS);
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> cards(TOTAL_CARDS, 0);
            cards[0] = 1;
          
            std::shuffle(cards.begin(), cards.end(), gen);
            
            bool found = false;
            for (int i = 0; i < DRAWN_CARDS; i++) {
                if (cards[i] == 1) {
                    found = true;
                    break;
                }
            }
            
            if (found) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 13: Поиск разыскиваемой фотокарточки среди 100 (извлечено 10)", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}