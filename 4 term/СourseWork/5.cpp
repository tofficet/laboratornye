#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <iomanip>
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <config.json>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << argv[1] << '\n';
        return 1;
    }

    json config;
    try {
        file >> config;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка JSON: " << e.what() << '\n';
        return 1;
    }

    double p = config.value("p", 0.5);
    double q = config.value("q", 1.0 - p);
    double s_plus = config.value("s_plus", 1.0);
    double s_minus = config.value("s_minus", 1.0);
    int maxN = config.value("maxN", 232);
    int numSimulations = config.value("num_simulations", 100000);

    if (p <= 0 || p >= 1 || q <= 0 || q >= 1 || std::abs(p + q - 1.0) > 1e-12) {
        std::cerr << "p и q должны быть в (0,1) и суммироваться в 1.\n";
        return 1;
    }
    if (s_plus <= 0 || s_minus <= 0) {
        std::cerr << "Размеры шагов должны быть положительными.\n";
        return 1;
    }
    if (maxN <= 0 || numSimulations <= 0) {
        std::cerr << "maxN и num_simulations должны быть положительными.\n";
        return 1;
    }

    std::cout << "Параметры:\n";
    std::cout << "  p = " << p << ", q = " << q << '\n';
    std::cout << "  s_plus = " << s_plus << ", s_minus = " << s_minus << '\n';
    std::cout << "  maxN = " << maxN << ", число симуляций = " << numSimulations << "\n\n";

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<long long> counts(maxN + 1, 0);

    for (int sim = 0; sim < numSimulations; ++sim) {
        double pos = 0.0;
        for (int step = 1; step <= maxN; ++step) {
            if (dist(rng) < p)
                pos += s_plus;
            else
                pos -= s_minus;

            if (std::abs(pos) < 1e-12) {
                counts[step]++;
                break;
            }
        }
    }

    std::cout << "=== Эмпирические вероятности первого возвращения ===\n";
    std::cout << "Шаг\tВероятность\tНакопленная\n";
    double cum = 0.0;
    for (int n = 1; n <= maxN; ++n) {
        double prob = static_cast<double>(counts[n]) / numSimulations;
        cum += prob;
        std::cout << n << "\t"
                  << std::fixed << std::setprecision(8) << prob << "\t"
                  << cum << '\n';
    }
    double never = 1.0 - cum;
    std::cout << "Не вернулись за " << maxN << " шагов: "
              << std::fixed << std::setprecision(8) << never << '\n';

    return 0;
}

// g++ -std=c++17 main.cpp -o random_walk -I/opt/homebrew/include
// ./random_walk config2.json