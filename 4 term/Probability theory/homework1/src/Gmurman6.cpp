#include "../include/factorial.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " k" << std::endl;
        return 1;
    }
    int K = std::stoi(argv[1]);
    if (K < 2) {
        std::cerr << "Ошибка: k должно быть >= 2" << std::endl;
        return 1;
    }
    
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int CUBES_PER_EDGE = 10;
    const int TOTAL_CUBES = 1000;
    
    int cubes_with_3 = 8;                                     
    int cubes_with_2 = 12 * (CUBES_PER_EDGE - 2);            
    int cubes_with_1 = 6 * (CUBES_PER_EDGE - 2) * (CUBES_PER_EDGE - 2); 
    int cubes_with_0 = (CUBES_PER_EDGE - 2) * (CUBES_PER_EDGE - 2) * (CUBES_PER_EDGE - 2); 
    
    double theoretical_one = static_cast<double>(cubes_with_1) / TOTAL_CUBES;   
    double theoretical_two = static_cast<double>(cubes_with_2) / TOTAL_CUBES;   
    double theoretical_three = static_cast<double>(cubes_with_3) / TOTAL_CUBES; 
    
    std::vector<double> empirical_one;
    std::vector<double> empirical_two;
    std::vector<double> empirical_three;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        int count_one = 0;
        int count_two = 0;
        int count_three = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double r = dist(gen) * TOTAL_CUBES;
            
            if (r < cubes_with_0) {
            }
            else if (r < cubes_with_0 + cubes_with_1) {
                count_one++;
            }
            else if (r < cubes_with_0 + cubes_with_1 + cubes_with_2) {
                count_two++;
            }
            else {
                count_three++;
            }
        }
        
        empirical_one.push_back(static_cast<double>(count_one) / numTrials);
        empirical_two.push_back(static_cast<double>(count_two) / numTrials);
        empirical_three.push_back(static_cast<double>(count_three) / numTrials);
    }
    
    printResults("ЗАДАЧА 6 (а): Куб распилен на 1000 кубиков. Вероятность 1 окрашенной грани", 
                 theoretical_one, trialCounts, empirical_one);
    
    printResults("ЗАДАЧА 6 (б): Куб распилен на 1000 кубиков. Вероятность 2 окрашенных граней", 
                 theoretical_two, trialCounts, empirical_two);
    
    printResults("ЗАДАЧА 6 (в): Куб распилен на 1000 кубиков. Вероятность 3 окрашенных граней", 
                 theoretical_three, trialCounts, empirical_three);
    
    return 0;
}