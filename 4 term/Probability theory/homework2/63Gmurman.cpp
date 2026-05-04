#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalOnePointPerPart() {
    return 2.0 / 9.0;
}

void printResults(const std::string& title, 
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность = " << std::fixed << std::setprecision(8) << theoretical << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | эмпир. = " << std::setprecision(8) << empirical[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    const double THEOR = theoreticalOnePointPerPart();
    
    std::vector<double> empirical;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        int successCount = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            double point1 = dis(gen);
            double point2 = dis(gen);
            double point3 = dis(gen);
            
            int part1 = (point1 < 1.0/3.0) ? 0 : ((point1 < 2.0/3.0) ? 1 : 2);
            int part2 = (point2 < 1.0/3.0) ? 0 : ((point2 < 2.0/3.0) ? 1 : 2);
            int part3 = (point3 < 1.0/3.0) ? 0 : ((point3 < 2.0/3.0) ? 1 : 2);
            
            if (part1 != part2 && part1 != part3 && part2 != part3) {
                successCount++;
            }
        }
        
        empirical.push_back(static_cast<double>(successCount) / numTrials);
    }
    
    printResults("ЗАДАЧА 63: Отрезок разделен на три равные части. Брошены три точки. Вероятность, что на каждую часть попадет по одной точке", 
                 THEOR, trialCounts, empirical);
    
    std::cout << "\nАналитическое вычисление:\n";
    std::cout << "Всего исходов: 3^3 = 27\n";
    std::cout << "Благоприятных исходов: 3! = 6\n";
    std::cout << "P = 6/27 = 2/9 = " << THEOR << std::endl;
    
    return 0;
}