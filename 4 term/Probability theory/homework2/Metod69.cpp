#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <map>

struct TheoreticalValues {
    double p1, p2, p3, p4;
};

TheoreticalValues theoretical36WithoutReturn() {
    double total = 58905.0;
    double p1 = 1152.0 / total;
    double p2 = 1296.0 / total;
    double p3 = 32256.0 / total;
    double p4 = 9.0 / total;
    return {p1, p2, p3, p4};
}

TheoreticalValues theoretical36WithReturn() {
    double total = 6561.0;
    double p1 = 288.0 / total;
    double p2 = 216.0 / total;
    double p3 = 3024.0 / total;
    double p4 = 9.0 / total;
    return {p1, p2, p3, p4};
}

TheoreticalValues theoretical52WithoutReturn() {
    double total = 270725.0;
    double p1 = 2496.0 / total;
    double p2 = 2808.0 / total;
    double p3 = 183040.0 / total;
    double p4 = 13.0 / total;
    return {p1, p2, p3, p4};
}

void printResults(const std::string& title, 
                  const TheoreticalValues& theor,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalP1,
                  const std::vector<double>& empiricalP2,
                  const std::vector<double>& empiricalP3,
                  const std::vector<double>& empiricalP4) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Теоретические вероятности:\n";
    std::cout << "P1 (3+1) = " << std::fixed << std::setprecision(8) << theor.p1 << std::endl;
    std::cout << "P2 (2+2) = " << theor.p2 << std::endl;
    std::cout << "P3 (разные) = " << theor.p3 << std::endl;
    std::cout << "P4 (каре) = " << theor.p4 << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    std::cout << std::left << std::setw(15) << "Испытаний"
              << std::right << std::setw(15) << "P1 эмп."
              << std::setw(15) << "P2 эмп."
              << std::setw(15) << "P3 эмп."
              << std::setw(15) << "P4 эмп." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << std::left << std::setw(15) << trialCounts[i]
                  << std::right << std::setw(15) << std::setprecision(8) << empiricalP1[i]
                  << std::setw(15) << empiricalP2[i]
                  << std::setw(15) << empiricalP3[i]
                  << std::setw(15) << empiricalP4[i] << std::endl;
    }
}

std::vector<int> getRanks36(int card) {
    return {(card % 9) + 6};
}

std::vector<int> getRanks52(int card) {
    return {(card % 13) + 2};
}

void simulate36WithoutReturn(const std::vector<int>& trialCounts,
                              std::vector<double>& empP1,
                              std::vector<double>& empP2,
                              std::vector<double>& empP3,
                              std::vector<double>& empP4) {
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int countP1 = 0, countP2 = 0, countP3 = 0, countP4 = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> deck(36);
            for (int i = 0; i < 36; i++) deck[i] = i;
            std::shuffle(deck.begin(), deck.end(), gen);
            
            std::vector<int> ranks;
            for (int i = 0; i < 4; i++) {
                ranks.push_back(deck[i] % 9);
            }
            std::sort(ranks.begin(), ranks.end());
            
            std::map<int, int> freq;
            for (int r : ranks) freq[r]++;
            
            std::vector<int> counts;
            for (auto& p : freq) counts.push_back(p.second);
            std::sort(counts.begin(), counts.end(), std::greater<int>());
            
            if (counts.size() == 2 && counts[0] == 3 && counts[1] == 1) countP1++;
            else if (counts.size() == 2 && counts[0] == 2 && counts[1] == 2) countP2++;
            else if (counts.size() == 4) countP3++;
            else if (counts.size() == 1 && counts[0] == 4) countP4++;
        }
        
        empP1.push_back(static_cast<double>(countP1) / numTrials);
        empP2.push_back(static_cast<double>(countP2) / numTrials);
        empP3.push_back(static_cast<double>(countP3) / numTrials);
        empP4.push_back(static_cast<double>(countP4) / numTrials);
    }
}

void simulate36WithReturn(const std::vector<int>& trialCounts,
                           std::vector<double>& empP1,
                           std::vector<double>& empP2,
                           std::vector<double>& empP3,
                           std::vector<double>& empP4) {
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 35);
        
        int countP1 = 0, countP2 = 0, countP3 = 0, countP4 = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> ranks;
            for (int i = 0; i < 4; i++) {
                ranks.push_back(dis(gen) % 9);
            }
            std::sort(ranks.begin(), ranks.end());
            
            std::map<int, int> freq;
            for (int r : ranks) freq[r]++;
            
            std::vector<int> counts;
            for (auto& p : freq) counts.push_back(p.second);
            std::sort(counts.begin(), counts.end(), std::greater<int>());
            
            if (counts.size() == 2 && counts[0] == 3 && counts[1] == 1) countP1++;
            else if (counts.size() == 2 && counts[0] == 2 && counts[1] == 2) countP2++;
            else if (counts.size() == 4) countP3++;
            else if (counts.size() == 1 && counts[0] == 4) countP4++;
        }
        
        empP1.push_back(static_cast<double>(countP1) / numTrials);
        empP2.push_back(static_cast<double>(countP2) / numTrials);
        empP3.push_back(static_cast<double>(countP3) / numTrials);
        empP4.push_back(static_cast<double>(countP4) / numTrials);
    }
}

void simulate52WithoutReturn(const std::vector<int>& trialCounts,
                              std::vector<double>& empP1,
                              std::vector<double>& empP2,
                              std::vector<double>& empP3,
                              std::vector<double>& empP4) {
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        
        int countP1 = 0, countP2 = 0, countP3 = 0, countP4 = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            std::vector<int> deck(52);
            for (int i = 0; i < 52; i++) deck[i] = i;
            std::shuffle(deck.begin(), deck.end(), gen);
            
            std::vector<int> ranks;
            for (int i = 0; i < 4; i++) {
                ranks.push_back(deck[i] % 13);
            }
            std::sort(ranks.begin(), ranks.end());
            
            std::map<int, int> freq;
            for (int r : ranks) freq[r]++;
            
            std::vector<int> counts;
            for (auto& p : freq) counts.push_back(p.second);
            std::sort(counts.begin(), counts.end(), std::greater<int>());
            
            if (counts.size() == 2 && counts[0] == 3 && counts[1] == 1) countP1++;
            else if (counts.size() == 2 && counts[0] == 2 && counts[1] == 2) countP2++;
            else if (counts.size() == 4) countP3++;
            else if (counts.size() == 1 && counts[0] == 4) countP4++;
        }
        
        empP1.push_back(static_cast<double>(countP1) / numTrials);
        empP2.push_back(static_cast<double>(countP2) / numTrials);
        empP3.push_back(static_cast<double>(countP3) / numTrials);
        empP4.push_back(static_cast<double>(countP4) / numTrials);
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    
    TheoreticalValues theor36Without = theoretical36WithoutReturn();
    std::vector<double> emp36WOP1, emp36WOP2, emp36WOP3, emp36WOP4;
    simulate36WithoutReturn(trialCounts, emp36WOP1, emp36WOP2, emp36WOP3, emp36WOP4);
    printResults("ЗАДАЧА 69: Колода 36 карт, выбор без возвращения", 
                 theor36Without, trialCounts, emp36WOP1, emp36WOP2, emp36WOP3, emp36WOP4);
    
    TheoreticalValues theor36With = theoretical36WithReturn();
    std::vector<double> emp36WP1, emp36WP2, emp36WP3, emp36WP4;
    simulate36WithReturn(trialCounts, emp36WP1, emp36WP2, emp36WP3, emp36WP4);
    printResults("ЗАДАЧА 69: Колода 36 карт, выбор с возвращением", 
                 theor36With, trialCounts, emp36WP1, emp36WP2, emp36WP3, emp36WP4);
    
    TheoreticalValues theor52Without = theoretical52WithoutReturn();
    std::vector<double> emp52WOP1, emp52WOP2, emp52WOP3, emp52WOP4;
    simulate52WithoutReturn(trialCounts, emp52WOP1, emp52WOP2, emp52WOP3, emp52WOP4);
    printResults("ЗАДАЧА 69: Колода 52 карты, выбор без возвращения", 
                 theor52Without, trialCounts, emp52WOP1, emp52WOP2, emp52WOP3, emp52WOP4);
    
    return 0;
}