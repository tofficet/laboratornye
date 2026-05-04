#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalSecondGivenStandard() {
    double pFirst = 0.55;
    double pSecond = 0.45;
    double pStandardFirst = 0.9;
    double pStandardSecond = 0.98;
    return (pSecond * pStandardSecond) / (pFirst * pStandardFirst + pSecond * pStandardSecond);
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность, что проверил второй товаровед = "
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

    const double THEOR = theoreticalSecondGivenStandard();

    std::vector<double> empirical;

    const double probFirst = 0.55;
    const double probRecognizeFirst = 0.9;
    const double probRecognizeSecond = 0.98;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> checkerDis(0.0, 1.0);
        std::uniform_real_distribution<> recognizeDis(0.0, 1.0);

        int recognizedCount = 0;
        int secondRecognizedCount = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            bool isFirst = (checkerDis(gen) < probFirst);
            double recognizeProb = isFirst ? probRecognizeFirst : probRecognizeSecond;

            if (recognizeDis(gen) < recognizeProb) {
                recognizedCount++;
                if (!isFirst) {
                    secondRecognizedCount++;
                }
            }
        }

        if (recognizedCount > 0) {
            empirical.push_back(static_cast<double>(secondRecognizedCount) / recognizedCount);
        } else {
            empirical.push_back(0.0);
        }
    }

    printResults("ЗАДАЧА 102: Два товароведа",
                 THEOR, trialCounts, empirical);

    return 0;
}