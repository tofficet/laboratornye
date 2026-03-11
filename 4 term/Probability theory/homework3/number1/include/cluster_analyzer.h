#ifndef CLUSTER_ANALYZER_H
#define CLUSTER_ANALYZER_H

#include <string>
#include <vector>
#include <random>
#include <stdexcept>

class ClusterAnalyzer {
public:
    struct DistributionParams {
        enum class Type { UNIFORM, CUSTOM } type;
        std::vector<double> probabilities;
    };

    ClusterAnalyzer(int clusterSize, int alphabetSize, 
                   const std::string& pattern, 
                   const DistributionParams& distParams);
    
    std::string generateCluster();
    
    bool areConnectable(const std::string& left, const std::string& right) const;
    
    void analyzeSeries(int M);
    
    double getProbabilityAllConnectable() const { return probabilityAllConnectable; }
    double getProbabilityDConnectable(int D) const;
    double getProbabilityAllNonConnectable() const { return probabilityAllNonConnectable; }
    
    const std::vector<std::string>& getGeneratedClusters() const { return clusters; }
    const std::vector<bool>& getConnectabilityMatrix() const { return connectabilityMatrix; }
    
private:
    int n; 
    int r;
    int k;
    std::string pattern;
    DistributionParams distParams;
    
    std::mt19937 rng;
    std::uniform_int_distribution<> uniformDist;
    std::discrete_distribution<> customDist;
    
    std::vector<std::string> clusters;
    std::vector<bool> connectabilityMatrix;
    
    double probabilityAllConnectable;
    double probabilityAllNonConnectable;
    
    char generateChar();
    bool isValidCluster(const std::string& cluster) const;
    void validatePattern() const;
};

#endif