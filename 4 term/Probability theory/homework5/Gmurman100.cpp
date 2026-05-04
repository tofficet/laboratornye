#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalFirstGivenError() {
    double pFirst = 0.5;
    double pSecond = 0.5;
    double pErrorFirst = 0.05;
    double pErrorSecond = 0.1;
    return (pFirst * pErrorFirst) / (pFirst * pErrorFirst + pSecond * pErrorSecond);
}

void printResults(const std::string& title,
                  double theoretical,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empirical) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность, что ошиблась первая перфораторщица = "
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

    const double THEOR = theoreticalFirstGivenError();

    std::vector<double> empirical;

    const double probErrorFirst = 0.05;
    const double probErrorSecond = 0.1;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> operatorDis(1, 2);
        std::uniform_real_distribution<> errorDis(0.0, 1.0);

        int errorCount = 0;
        int firstErrorCount = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            int op = operatorDis(gen);
            double errorProb = (op == 1) ? probErrorFirst : probErrorSecond;

            if (errorDis(gen) < errorProb) {
                errorCount++;
                if (op == 1) {
                    firstErrorCount++;
                }
            }
        }

        if (errorCount > 0) {
            empirical.push_back(static_cast<double>(firstErrorCount) / errorCount);
        } else {
            empirical.push_back(0.0);
        }
    }

    printResults("ЗАДАЧА 100: Две перфораторщицы",
                 THEOR, trialCounts, empirical);

    return 0;
}