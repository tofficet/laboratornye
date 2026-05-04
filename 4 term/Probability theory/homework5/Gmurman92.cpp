#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalProbability() {
    return (3.0 / 5.0) * 0.95 + (2.0 / 5.0) * 0.7;
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность поражения мишени = "
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

    const int totalRifles = 5;
    const int scopedRifles = 3;

    const double probHitScoped = 0.95;
    const double probHitUnscoped = 0.7;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> rifleDis(1, totalRifles);
        std::uniform_real_distribution<> hitDis(0.0, 1.0);

        int success = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            int rifle = rifleDis(gen);
            double hitProb = (rifle <= scopedRifles) ? probHitScoped : probHitUnscoped;

            if (hitDis(gen) < hitProb) {
                success++;
            }
        }

        empirical.push_back(static_cast<double>(success) / numTrials);
    }

    printResults("ЗАДАЧА 92: Винтовки с оптическим прицелом и без",
                 THEOR, trialCounts, empirical);

    return 0;
}