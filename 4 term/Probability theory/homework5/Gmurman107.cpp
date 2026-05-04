#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalThirdGivenTwoHits() {
    double p1 = 0.6, p2 = 0.5, p3 = 0.4;
    double q1 = 1.0 - p1, q2 = 1.0 - p2, q3 = 1.0 - p3;
    
    double pTwoHitsAndThird = p3 * (p1 * q2 + q1 * p2);
    double pTwoHits = p1 * p2 * q3 + p1 * q2 * p3 + q1 * p2 * p3;
    
    return pTwoHitsAndThird / pTwoHits;
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность, что третий стрелок попал = "
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

    const double THEOR = theoreticalThirdGivenTwoHits();

    std::vector<double> empirical;

    const double prob1 = 0.6;
    const double prob2 = 0.5;
    const double prob3 = 0.4;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> hitDis(0.0, 1.0);

        int twoHitsCount = 0;
        int thirdHitCount = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            bool hit1 = (hitDis(gen) < prob1);
            bool hit2 = (hitDis(gen) < prob2);
            bool hit3 = (hitDis(gen) < prob3);

            int totalHits = hit1 + hit2 + hit3;

            if (totalHits == 2) {
                twoHitsCount++;
                if (hit3) {
                    thirdHitCount++;
                }
            }
        }

        if (twoHitsCount > 0) {
            empirical.push_back(static_cast<double>(thirdHitCount) / twoHitsCount);
        } else {
            empirical.push_back(0.0);
        }
    }

    printResults("ЗАДАЧА 107: Три стрелка, два попадания",
                 THEOR, trialCounts, empirical);

    return 0;
}