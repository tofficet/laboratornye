#include "../include/cluster_analyzer.h"
#include <algorithm>
#include <iostream>
#include <cmath>

ClusterAnalyzer::ClusterAnalyzer(int clusterSize, int alphabetSize,
                               const std::string& pattern,
                               const DistributionParams& distParams)
    : n(clusterSize), r(alphabetSize), pattern(pattern), distParams(distParams),
      k(pattern.length()), rng(std::random_device{}()) {
    
    validatePattern();
    
    if (distParams.type == DistributionParams::Type::UNIFORM) {
        uniformDist = std::uniform_int_distribution<>(0, r - 1);
    } else {
        if (distParams.probabilities.size() != static_cast<size_t>(r)) {
            throw std::invalid_argument("Number of probabilities must equal alphabet size");
        }
        double sum = 0;
        for (double p : distParams.probabilities) {
            if (p < 0 || p > 1) throw std::invalid_argument("Probabilities must be in [0,1]");
            sum += p;
        }
        if (std::abs(sum - 1.0) > 1e-6) {
            throw std::invalid_argument("Probabilities must sum to 1");
        }
        customDist = std::discrete_distribution<>(distParams.probabilities.begin(), 
                                                  distParams.probabilities.end());
    }
}

void ClusterAnalyzer::validatePattern() const {
    if (pattern.empty()) {
        throw std::invalid_argument("Pattern cannot be empty");
    }
    for (char c : pattern) {
        if (c < '0' || c >= '0' + r) {
            throw std::invalid_argument("Pattern contains invalid characters for alphabet");
        }
    }
}

bool ClusterAnalyzer::isValidCluster(const std::string& cluster) const {
    if (cluster.length() != static_cast<size_t>(n)) return false;
    for (char c : cluster) {
        if (c < '0' || c >= '0' + r) return false;
    }
    return true;
}

char ClusterAnalyzer::generateChar() {
    if (distParams.type == DistributionParams::Type::UNIFORM) {
        return '0' + uniformDist(rng);
    } else {
        return '0' + customDist(rng);
    }
}

std::string ClusterAnalyzer::generateCluster() {
    std::string cluster;
    cluster.reserve(n);
    for (int i = 0; i < n; ++i) {
        cluster += generateChar();
    }
    return cluster;
}

bool ClusterAnalyzer::areConnectable(const std::string& left, const std::string& right) const {
    if (!isValidCluster(left) || !isValidCluster(right)) {
        throw std::invalid_argument("Invalid cluster strings");
    }
    
    for (int leftPart = 1; leftPart <= k - 1; ++leftPart) {
        int rightPart = k - leftPart;
        
        if (leftPart > n || rightPart > n) continue;
        
        std::string leftEnd = left.substr(n - leftPart, leftPart);
        std::string rightStart = right.substr(0, rightPart);
        std::string combined = leftEnd + rightStart;
        
        if (combined == pattern) {
            return true;
        }
    }
    return false;
}

void ClusterAnalyzer::analyzeSeries(int M) {
    if (M <= 0) {
        throw std::invalid_argument("Number of clusters must be positive");
    }
    
    clusters.clear();
    clusters.reserve(M);
    for (int i = 0; i < M; ++i) {
        clusters.push_back(generateCluster());
    }
    
    connectabilityMatrix.clear();
    int matrixSize = M * (M - 1) / 2;
    connectabilityMatrix.reserve(matrixSize);
    
    int connectableCount = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = i + 1; j < M; ++j) {
            bool connectable = areConnectable(clusters[i], clusters[j]);
            connectabilityMatrix.push_back(connectable);
            if (connectable) {
                connectableCount++;
            }
        }
    }
    
    int totalPairs = M * (M - 1) / 2;
    if (totalPairs > 0) {
        probabilityAllConnectable = (connectableCount == totalPairs) ? 1.0 : 0.0;
        probabilityAllNonConnectable = (connectableCount == 0) ? 1.0 : 0.0;
    } else {
        probabilityAllConnectable = 0.0;
        probabilityAllNonConnectable = 0.0;
    }
}

double ClusterAnalyzer::getProbabilityDConnectable(int D) const {
    if (D < 0 || D > static_cast<int>(clusters.size() * (clusters.size() - 1) / 2)) {
        return 0.0;
    }
    
    int connectableCount = std::count(connectabilityMatrix.begin(), 
                                      connectabilityMatrix.end(), true);
    
    if (D == connectableCount) {
        return 1.0;
    }
    
    
    return 0.0;
}