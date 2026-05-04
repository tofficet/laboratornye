#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalProbability() {
    return (12.0 / 50.0) * 0.9 + (20.0 / 50.0) * 0.6 + (18.0 / 50.0) * 0.9;
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность отличного качества = "
              << std::fixed << std::setprecision(6) << theoretical << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;

    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i]
                  << " | эмпир. вероятность = "
                  << std::fixed << std::setprecision(6) << empirical[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};

    const double THEOR = theoreticalProbability();

    std::vector<double> empirical;

    const int totalParts = 50;
    const int factory1Count = 12;
    const int factory2Count = 20;

    const double probExcellent1 = 0.9;
    const double probExcellent2 = 0.6;
    const double probExcellent3 = 0.9;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> partDis(1, totalParts);
        std::uniform_real_distribution<> qualityDis(0.0, 1.0);

        int success = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            int part = partDis(gen);
            double probExcellent;

            if (part <= factory1Count) {
                probExcellent = probExcellent1;
            } else if (part <= factory1Count + factory2Count) {
                probExcellent = probExcellent2;
            } else {
                probExcellent = probExcellent3;
            }

            if (qualityDis(gen) < probExcellent) {
                success++;
            }
        }

        empirical.push_back(static_cast<double>(success) / numTrials);
    }

    printResults("ЗАДАЧА 93: Детали с трёх заводов",
                 THEOR, trialCounts, empirical);

    return 0;
}