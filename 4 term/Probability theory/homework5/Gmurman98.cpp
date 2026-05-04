#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

double theoreticalWithScope() {
    double pScope = 4.0 / 10.0;
    double pNoScope = 6.0 / 10.0;
    double pScopeGivenHit = (pScope * 0.95) / (pScope * 0.95 + pNoScope * 0.8);
    return pScopeGivenHit;
}

double theoreticalWithoutScope() {
    double pScope = 4.0 / 10.0;
    double pNoScope = 6.0 / 10.0;
    double pNoScopeGivenHit = (pNoScope * 0.8) / (pScope * 0.95 + pNoScope * 0.8);
    return pNoScopeGivenHit;
}

void printResults(const std::string& title,
                  double theorScope, double theorNoScope,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empScope,
                  const std::vector<double>& empNoScope) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность, что с прицелом = "
              << std::fixed << std::setprecision(6) << theorScope << std::endl;
    std::cout << "Теоретическая вероятность, что без прицела = "
              << std::fixed << std::setprecision(6) << theorNoScope << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;

    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] << std::endl;
        std::cout << "  с прицелом | эмпир. = " << std::fixed << std::setprecision(6) << empScope[i] << std::endl;
        std::cout << "  без прицела | эмпир. = " << empNoScope[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};

    const double THEOR_SCOPE = theoreticalWithScope();
    const double THEOR_NO_SCOPE = theoreticalWithoutScope();

    std::vector<double> empiricalScope;
    std::vector<double> empiricalNoScope;

    const int totalRifles = 10;
    const int scopedRifles = 4;

    const double probHitScoped = 0.95;
    const double probHitUnscoped = 0.8;

    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> rifleDis(1, totalRifles);
        std::uniform_real_distribution<> hitDis(0.0, 1.0);

        int hitCount = 0;
        int scopeGivenHit = 0;

        for (int trial = 0; trial < numTrials; trial++) {
            int rifle = rifleDis(gen);
            bool isScoped = (rifle <= scopedRifles);
            double hitProb = isScoped ? probHitScoped : probHitUnscoped;

            if (hitDis(gen) < hitProb) {
                hitCount++;
                if (isScoped) {
                    scopeGivenHit++;
                }
            }
        }

        if (hitCount > 0) {
            empiricalScope.push_back(static_cast<double>(scopeGivenHit) / hitCount);
            empiricalNoScope.push_back(1.0 - static_cast<double>(scopeGivenHit) / hitCount);
        } else {
            empiricalScope.push_back(0.0);
            empiricalNoScope.push_back(0.0);
        }
    }

    printResults("ЗАДАЧА 98: Что вероятнее после попадания",
                 THEOR_SCOPE, THEOR_NO_SCOPE, trialCounts, empiricalScope, empiricalNoScope);

    return 0;
}