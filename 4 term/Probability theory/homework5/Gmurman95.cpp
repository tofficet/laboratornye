#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalProbability() {
    return 0.4;
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность извлечь белый шар из третьей урны = "
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

    const int totalBalls = 10;
    const int blackBalls = 6;
    const int whiteBalls = 4;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> ballDis(1, totalBalls);
        std::uniform_int_distribution<> urn2Dis(1, totalBalls + 1);
        std::uniform_int_distribution<> urn3Dis(1, totalBalls + 1);

        int success = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            int urn2_black = blackBalls;
            int urn2_white = whiteBalls;
            int urn3_black = blackBalls;
            int urn3_white = whiteBalls;

            int drawn1 = ballDis(gen);
            if (drawn1 <= whiteBalls) {
                urn2_white++;
            } else {
                urn2_black++;
            }

            int totalUrn2 = urn2_black + urn2_white;
            std::uniform_int_distribution<> urn2DrawDis(1, totalUrn2);
            int drawn2 = urn2DrawDis(gen);
            if (drawn2 <= urn2_white) {
                urn3_white++;
            } else {
                urn3_black++;
            }

            int totalUrn3 = urn3_black + urn3_white;
            std::uniform_int_distribution<> urn3DrawDis(1, totalUrn3);
            int drawn3 = urn3DrawDis(gen);
            if (drawn3 <= urn3_white) {
                success++;
            }
        }

        empirical.push_back(static_cast<double>(success) / numTrials);
    }

    printResults("ЗАДАЧА 95: Шары в трёх урнах",
                 THEOR, trialCounts, empirical);

    return 0;
}