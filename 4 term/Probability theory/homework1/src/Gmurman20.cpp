#include "../include/factorial.hpp"

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const int TOTAL_STUDENTS = 12;
    const int EXCELLENT_STUDENTS = 8;
    const int OTHER_STUDENTS = TOTAL_STUDENTS - EXCELLENT_STUDENTS; 
    const int CHOSEN_STUDENTS = 9;
    const int DESIRED_EXCELLENT = 5;
    const int DESIRED_OTHER = CHOSEN_STUDENTS - DESIRED_EXCELLENT; 
    
    // Теоретическая вероятность: C(8,5) * C(4,4) / C(12,9)
    double theoretical = combination(EXCELLENT_STUDENTS, DESIRED_EXCELLENT) / 
                          combination(TOTAL_STUDENTS, CHOSEN_STUDENTS);
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> students(TOTAL_STUDENTS);
            for (int i = 0; i < TOTAL_STUDENTS; i++) {
                students[i] = (i < EXCELLENT_STUDENTS) ? 1 : 0;
            }
            
            std::shuffle(students.begin(), students.end(), gen);
            
            int excellentCount = 0;
            for (int i = 0; i < CHOSEN_STUDENTS; i++) {
                if (students[i] == 1) {
                    excellentCount++;
                }
            }
            
            if (excellentCount == DESIRED_EXCELLENT) {
                successCount++;
            }
        }
        
        double emp = static_cast<double>(successCount) / numTrials;
        empirical.push_back(emp);
    }
    
    printResults("ЗАДАЧА 20: 12 студентов (8 отличников). Вероятность, что среди 9 выбранных ровно 5 отличников", 
                 theoretical, trialCounts, empirical);
    
    return 0;
}