#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <chrono>
#include <cmath>

class CoinExperiment {
private:
    int n;
    int k;
    std::mt19937_64 rng;
    std::uniform_int_distribution<int> coinSelector;
    std::uniform_int_distribution<int> flipSelector;
    
public:
    CoinExperiment(int totalCoins, int doubleHeads) 
        : n(totalCoins), k(doubleHeads), 
          rng(std::random_device{}()),
          coinSelector(0, n - 1),
          flipSelector(0, 1) {}
    
    bool flipCoin(bool isDoubleHeaded) {
        if (isDoubleHeaded) return true;
        return flipSelector(rng) == 0;
    }
    
    bool isDoubleHeadedCoin(int coinIndex) {
        return coinIndex < k;
    }
    
    std::vector<bool> runExperiment() {
        int coinIndex = coinSelector(rng);
        bool doubleHeaded = isDoubleHeadedCoin(coinIndex);
        
        std::vector<bool> results;
        for (int i = 0; i < 4; ++i) {
            results.push_back(flipCoin(doubleHeaded));
        }
        return results;
    }
    
    bool firstThreeHeads(const std::vector<bool>& results) {
        return results[0] && results[1] && results[2];
    }
    
    bool allFourHeads(const std::vector<bool>& results) {
        return results[0] && results[1] && results[2] && results[3];
    }
    
    void runExperiment(int numExperiments) {
        int countFirstThreeHeads = 0;
        int countAllFourHeads = 0;
        
        for (int i = 0; i < numExperiments; ++i) {
            auto results = runExperiment();
            
            if (firstThreeHeads(results)) {
                countFirstThreeHeads++;
                if (allFourHeads(results)) {
                    countAllFourHeads++;
                }
            }
        }
        
        double empirical = 0.0;
        if (countFirstThreeHeads > 0) {
            empirical = static_cast<double>(countAllFourHeads) / countFirstThreeHeads;
        }
        
        double theoretical = theoreticalProbability();
        
        std::cout << "\n=== Задача 2: Монеты (k двуголовых из n) ===" << std::endl;
        std::cout << "n = " << n << ", k = " << k << std::endl;
        std::cout << std::endl;
        std::cout << "Результаты после " << numExperiments << " испытаний:" << std::endl;
        std::cout << "первые три орла: " << countFirstThreeHeads << " раз" << std::endl;
        std::cout << "условная вероятность (4-й орёл | первые три орла): " 
                  << std::fixed << std::setprecision(6) << empirical << std::endl;
        std::cout << "теоретическая вероятность: " 
                  << std::fixed << std::setprecision(6) << theoretical << std::endl;
    }
    
    double theoreticalProbability() const {
        return static_cast<double>(15 * k + n) / (14 * k + 2 * n);
    }
};

class BarrelSimulation {
private:
    double p;
    double p1;
    int n;
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> dist;
    
public:
    BarrelSimulation(double hitProb, double explosionProb, int shots) 
        : p(hitProb), p1(explosionProb), n(shots),
          rng(std::random_device{}()),
          dist(0.0, 1.0) {}
    
    bool simulateShot() {
        return dist(rng) < p;
    }
    
    bool simulateExperiment() {
        int hits = 0;
        
        for (int i = 0; i < n; ++i) {
            if (simulateShot()) {
                hits++;
                if (hits >= 2) return true;
            }
        }
        
        if (hits == 1) {
            return dist(rng) < p1;
        }
        
        return false;
    }
    
    double theoreticalProbability() const {
        double p0 = std::pow(1.0 - p, n);
        double p1_prob = n * p * std::pow(1.0 - p, n - 1);
        double p2plus = 1.0 - p0 - p1_prob;
        
        return p1_prob * p1 + p2plus;
    }
    
    void runSimulation(int numExperiments) {
        int explosions = 0;
        int zeroHits = 0;
        int oneHit = 0;
        int twoPlusHits = 0;
        
        for (int i = 0; i < numExperiments; ++i) {
            int hits = 0;
            bool exploded = false;
            
            for (int j = 0; j < n; ++j) {
                if (simulateShot()) {
                    hits++;
                    if (hits >= 2) {
                        exploded = true;
                        break;
                    }
                }
            }
            
            if (hits == 0) zeroHits++;
            else if (hits == 1) {
                oneHit++;
                if (dist(rng) < p1) {
                    exploded = true;
                }
            } else {
                twoPlusHits++;
                exploded = true;
            }
            
            if (exploded) explosions++;
        }
        
        double empirical = static_cast<double>(explosions) / numExperiments;
        double theoretical = theoreticalProbability();
        
        std::cout << "\n=== Задача 3: Бочка с бензином ===" << std::endl;
        std::cout << "p = " << std::fixed << std::setprecision(3) << p << std::endl;
        std::cout << "p₁ = " << std::fixed << std::setprecision(3) << p1 << std::endl;
        std::cout << "n = " << n << std::endl;
        std::cout << std::endl;
        std::cout << "Результаты после " << numExperiments << " экспериментов:" << std::endl;
        std::cout << "  Теоретическая вероятность взрыва: " 
                  << std::fixed << std::setprecision(6) << theoretical << std::endl;
        std::cout << "  Эмпирическая вероятность взрыва: " 
                  << std::fixed << std::setprecision(6) << empirical << std::endl;
        std::cout << "  Разница: " << std::fixed << std::setprecision(6) 
                  << std::abs(theoretical - empirical) << std::endl;
        std::cout << std::endl;
    }
};

int main() {
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<int> dist(1, 3);
    
    int numExperiments;
    std::cout << "Введите количество испытаний: ";
    std::cin >> numExperiments;
    
    std::cout << "Выберите задачу (1, 2 или 3): ";
    int task;
    std::cin >> task;
    
    if (task == 1) {
        std::cout << "\n=== Задача 1: Шифр Вернама ===" << std::endl;
        std::cout << "N = 65536 (16-битное пространство ключей)" << std::endl;
        std::cout << std::endl;
        
        std::vector<int> kValues = {50, 100, 150, 200, 213, 250, 300};
        
        std::cout << std::setw(10) << "k" 
                  << std::setw(20) << "Теоретическая"
                  << std::setw(20) << "Эмпирическая"
                  << std::setw(15) << "Разница" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        
        for (int k : kValues) {
            double theoretical = 1.0 - std::exp(-static_cast<double>(k * k) / 65536.0);
            
            int collisions = 0;
            int trials = std::min(10000, numExperiments);
            
            for (int t = 0; t < trials; ++t) {
                std::uniform_int_distribution<uint16_t> keyDist(0, 65535);
                std::vector<uint16_t> X, Y;
                
                for (int i = 0; i < k; ++i) {
                    X.push_back(keyDist(rng));
                    Y.push_back(keyDist(rng));
                }
                
                bool found = false;
                for (int i = 0; i < k && !found; ++i) {
                    for (int j = 0; j < k && !found; ++j) {
                        if (X[i] == Y[j]) {
                            found = true;
                        }
                    }
                }
                
                if (found) collisions++;
            }
            
            double empirical = static_cast<double>(collisions) / trials;
            double diff = std::abs(theoretical - empirical);
            
            std::cout << std::setw(10) << k
                      << std::setw(20) << std::fixed << std::setprecision(6) << theoretical
                      << std::setw(20) << std::fixed << std::setprecision(6) << empirical
                      << std::setw(15) << std::fixed << std::setprecision(6) << diff
                      << std::endl;
        }
        
        std::cout << std::endl;
        std::cout << "При k > sqrt(N * ln(2)) ≈ 213 вероятность > 0.5" << std::endl;
        
    } else if (task == 2) {
        int n, k;
        std::cout << "Введите n (общее количество монет): ";
        std::cin >> n;
        std::cout << "Введите k (количество двуголовых монет): ";
        std::cin >> k;
        
        if (k < 0 || k > n) {
            std::cout << "Ошибка: k должно быть в диапазоне [0, n]" << std::endl;
            return 1;
        }
        
        CoinExperiment experiment(n, k);
        experiment.runExperiment(numExperiments);
        
    } else if (task == 3) {
        double p, p1;
        int n;
        
        std::cout << "Введите вероятность попадания одной пули (p): ";
        std::cin >> p;
        std::cout << "Введите вероятность взрыва при одном попадании (p₁): ";
        std::cin >> p1;
        std::cout << "Введите количество выстрелов (n): ";
        std::cin >> n;
        
        if (p < 0 || p > 1 || p1 < 0 || p1 > 1 || n <= 0) {
            std::cout << "Ошибка: некорректные параметры!" << std::endl;
            return 1;
        }
        
        BarrelSimulation sim(p, p1, n);
        sim.runSimulation(numExperiments);
        
    } else {
        std::cout << "Неверный выбор задачи!" << std::endl;
    }
    
    return 0;
}