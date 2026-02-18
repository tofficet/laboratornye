#include "../include/factorial.hpp"

int countEven(int N) {
    return N / 2; 
}

void runExperimentA(int N, int numTrials) {
    
    int K = countEven(N);
    
    double theor_first_even = static_cast<double>(K) / N;
    double theor_second_even = static_cast<double>(K) / N; 
    double theor_both_even = (static_cast<double>(K) / N) * (static_cast<double>(K-1) / (N-1));
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    
    int count_first_even = 0;
    int count_second_even = 0;
    int count_both_even = 0;
    
    for (int trial = 0; trial < numTrials; trial++) {
        std::uniform_int_distribution<> first_dist(1, N);
        int first = first_dist(gen);
        bool first_even = (first % 2 == 0);
        
        int second;
        do {
            std::uniform_int_distribution<> second_dist(1, N);
            second = second_dist(gen);
        } while (second == first);
        bool second_even = (second % 2 == 0);
        
        if (first_even) count_first_even++;
        if (second_even) count_second_even++;
        if (first_even && second_even) count_both_even++;
    }
    
    double emp_first_even = static_cast<double>(count_first_even) / numTrials;
    double emp_second_even = static_cast<double>(count_second_even) / numTrials;
    double emp_both_even = static_cast<double>(count_both_even) / numTrials;
    
    printResults1("ВАРИАНТ А (индексы не изменяются)", N, numTrials,
             theor_first_even, theor_second_even, theor_both_even,
             emp_first_even, emp_second_even, emp_both_even);

}

void runExperimentB(int N, int numTrials) {
    
    int K = countEven(N);
    
    double theor_first_even = static_cast<double>(K) / N;
    
    int evenNewPositions = (N-1) / 2;
    double theor_second_even = static_cast<double>(evenNewPositions) / (N-1);
    
    double theor_both_even = theor_first_even * theor_second_even;
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    
    int count_first_even = 0;
    int count_second_even = 0;
    int count_both_even = 0;
    
    for (int trial = 0; trial < numTrials; trial++) {
        std::vector<int> symbols(N);
        for (int i = 0; i < N; i++) {
            symbols[i] = i + 1;
        }
        
        std::uniform_int_distribution<> first_idx_dist(0, symbols.size() - 1);
        int first_idx = first_idx_dist(gen);
        int first_value = symbols[first_idx];
        bool first_even = (first_value % 2 == 0);
        
        symbols.erase(symbols.begin() + first_idx);
        
        std::vector<int> new_positions(symbols.size());
        for (size_t i = 0; i < symbols.size(); i++) {
            new_positions[i] = i + 1;
        }
        
        std::uniform_int_distribution<> second_idx_dist(0, symbols.size() - 1);
        int second_idx = second_idx_dist(gen);
        int second_new_number = new_positions[second_idx];
        bool second_even = (second_new_number % 2 == 0);
        
        if (first_even) count_first_even++;
        if (second_even) count_second_even++;
        if (first_even && second_even) count_both_even++;
    }
    
    double emp_first_even = static_cast<double>(count_first_even) / numTrials;
    double emp_second_even = static_cast<double>(count_second_even) / numTrials;
    double emp_both_even = static_cast<double>(count_both_even) / numTrials;
    
    printResults1("ВАРИАНТ B (индексы изменяются)", N, numTrials,
             theor_first_even, theor_second_even, theor_both_even,
             emp_first_even, emp_second_even, emp_both_even);
}

int main(int argc, char* argv[]) {
    int N = 10;
    int numTrials = 10000;
    
    if (argc >= 2) {
        N = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        numTrials = std::stoi(argv[2]);
    }
    
    runExperimentA(N, numTrials);
    runExperimentB(N, numTrials);
    
    return 0;
}