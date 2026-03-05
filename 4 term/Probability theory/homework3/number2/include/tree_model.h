#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <vector>
#include <random>
#include <memory>
#include <QString>
#include <QColor>

struct TreeNode {
    int id;
    int level;
    std::vector<int> children;
    double stopProbability;
    std::vector<double> moveProbabilities;
    
    TreeNode() : id(-1), level(0), stopProbability(0.0) {}
};

struct PathPoint {
    int nodeId;
    int level;
    QString nodeName;
    QColor color;
    
    PathPoint(int id, int lvl, const QString& name, QColor col = Qt::blue)
        : nodeId(id), level(lvl), nodeName(name), color(col) {}
};

class TreeModel {
public:
    TreeModel(int branchingFactor, int depth, 
              const std::vector<double>& levelStopProbabilities,
              const std::vector<std::vector<double>>& levelMoveProbabilities);
    
    std::vector<PathPoint> runSimulation();
    void runMultipleSimulations(int numExperiments);
    
    double getProbabilityToLeaf(int leafId) const;
    double getProbabilityToPassVertices(int verticesCount) const;
    int getTotalLeaves() const { return leafIds.size(); }
    int getTotalVertices() const { return nodes.size(); }
    
    const std::vector<TreeNode>& getNodes() const { return nodes; }
    const std::vector<int>& getLeafIds() const { return leafIds; }
    const std::vector<int>& getFinalNodeIds() const { return finalNodeIds; }
    const std::vector<int>& getPathLengths() const { return pathLengths; }
    int getLastExperimentPathLength() const { return lastPathLength; }
    const std::vector<int>& getLastExperimentPath() const { return lastPath; }
    
    QString getStatistics() const;

private:
    int M;
    int H;
    std::vector<TreeNode> nodes;
    std::vector<int> leafIds;
    
    std::vector<double> stopProbs;
    std::vector<std::vector<double>> moveProbs;
    
    std::vector<int> finalNodeIds;
    std::vector<int> pathLengths;
    
    std::vector<int> lastPath;
    int lastPathLength;
    
    std::mt19937 rng;
    
    void buildTree();
    int getNextNode(int currentNode, double randomValue);
    bool shouldStop(int nodeId, double randomValue);
    QString nodeIdToName(int id) const;
};

#endif