#include "../include/factorial.hpp"

int main(){
    std::vector<int> trialCounts={1000, 10000, 100000};

    int K=10;

    double theoretical_k = (1.0-1.0/pow(2.0, K-2));
    double theoretical_even = static_cast<double>(2.0/3.0);

    std::vector<double> emperical_k;
    std::vector<double> emperical_even;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen (seed);
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (int numTrials : trialCounts){

        int count_k=0, count_even=0;

        for (int trial =0; trial < numTrials; trial++){

            int steps = 1;
            double value=dist(gen);
            int prev = (value<0.5) ? 0 :1;

            while(true){
                value=dist(gen);
                int curr = (value<0.5) ? 0 :1;
                steps++;
                if (curr==prev) break;
                prev=curr;

            }
            if (steps<K) count_k++;
            if (steps%2==0) count_even++;
            
        }
        emperical_k.push_back(static_cast<double>(count_k)/ numTrials);
        emperical_even.push_back(static_cast<double>(count_even)/ numTrials);

    }

    printResults("Вероятность того, что опыт закончится до k-го бросания",
        theoretical_k,trialCounts,emperical_k);
    printResults("Вероятность того, что потребуется чётное число бросаний",
        theoretical_even,trialCounts,emperical_even);
    return 0;
}