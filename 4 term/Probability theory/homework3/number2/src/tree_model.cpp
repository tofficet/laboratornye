#include "tree_model.h"
#include <cmath>
#include <queue>
#include <algorithm>
#include <QDebug>

TreeModel::TreeModel(int branchingFactor, int depth,
                     const std::vector<double>& levelStopProbabilities,
                     const std::vector<std::vector<double>>& levelMoveProbabilities)
    : M(branchingFactor), H(depth), 
      stopProbs(levelStopProbabilities), 
      moveProbs(levelMoveProbabilities),
      rng(std::random_device{}()),
      lastPathLength(0) {
    
    // Проверка входных данных
    if (stopProbs.size() != static_cast<size_t>(depth + 1)) {
        throw std::invalid_argument("Количество вероятностей остановки должно быть равно глубине + 1");
    }
    
    if (moveProbs.size() != static_cast<size_t>(depth + 1)) {
        throw std::invalid_argument("Количество распределений перемещения должно быть равно глубине + 1");
    }
    
    for (size_t level = 0; level < moveProbs.size(); ++level) {
        if (moveProbs[level].size() != static_cast<size_t>(branchingFactor)) {
            throw std::invalid_argument("На каждом уровне должно быть M вероятностей перемещения");
        }
        
        // Проверка суммы вероятностей (должна быть 1)
        double sum = 0.0;
        for (double p : moveProbs[level]) {
            if (p < 0 || p > 1) {
                throw std::invalid_argument("Вероятности должны быть в [0, 1]");
            }
            sum += p;
        }
        if (std::abs(sum - 1.0) > 1e-6) {
            throw std::invalid_argument("Сумма вероятностей перемещения должна быть 1");
        }
    }
    
    buildTree();
}

void TreeModel::buildTree() {
    nodes.clear();
    leafIds.clear();
    
    int totalNodes = 0;
    for (int level = 0; level <= H; ++level) {
        totalNodes += static_cast<int>(pow(M, level));
    }
    
    nodes.resize(totalNodes);
    
    // Создаём узлы уровня за уровнем
    int nodeIndex = 0;
    for (int level = 0; level <= H; ++level) {
        int nodesAtLevel = static_cast<int>(pow(M, level));
        
        for (int i = 0; i < nodesAtLevel; ++i) {
            TreeNode& node = nodes[nodeIndex];
            node.id = nodeIndex;
            node.level = level;
            node.stopProbability = stopProbs[level];
            node.moveProbabilities = moveProbs[level];
            
            // Добавляем детей (кроме листьев)
            if (level < H) {
                for (int child = 0; child < M; ++child) {
                    int childId = nodeIndex + nodesAtLevel + i * M + child;
                    node.children.push_back(childId);
                }
            } else {
                // Это лист
                leafIds.push_back(nodeIndex);
            }
            
            nodeIndex++;
        }
    }
}

QString TreeModel::nodeIdToName(int id) const {
    if (id < 0 || id >= static_cast<int>(nodes.size())) {
        return "Invalid";
    }
    
    const TreeNode& node = nodes[id];
    if (node.level == 0) {
        return "Root";
    }
    
    // Находим позицию узла среди узлов своего уровня
    int nodesBeforeLevel = 0;
    for (int l = 0; l < node.level; ++l) {
        nodesBeforeLevel += static_cast<int>(pow(M, l));
    }
    
    int position = id - nodesBeforeLevel;
    return QString("L%1_%2").arg(node.level).arg(position);
}

int TreeModel::getNextNode(int currentNode, double randomValue) {
    const TreeNode& node = nodes[currentNode];
    
    if (node.children.empty()) {
        return currentNode; // лист
    }
    
    // Выбираем ребёнка на основе вероятностей
    double cumulative = 0.0;
    for (size_t i = 0; i < node.moveProbabilities.size(); ++i) {
        cumulative += node.moveProbabilities[i];
        if (randomValue < cumulative) {
            return node.children[i];
        }
    }
    
    // На случай ошибок округления
    return node.children.back();
}

bool TreeModel::shouldStop(int nodeId, double randomValue) {
    const TreeNode& node = nodes[nodeId];
    return randomValue < node.stopProbability;
}

std::vector<PathPoint> TreeModel::runSimulation() {
    std::vector<PathPoint> path;
    lastPath.clear();
    
    int currentNode = 0; // начинаем с корня
    lastPath.push_back(currentNode);
    
    // Добавляем корень в путь
    path.push_back(PathPoint(currentNode, nodes[currentNode].level, 
                             nodeIdToName(currentNode), Qt::blue));
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    while (true) {
        // Проверяем, не лист ли это
        if (nodes[currentNode].children.empty()) {
            break; // дошли до листа
        }
        
        // Проверяем, не останавливается ли точка
        double stopRand = dist(rng);
        if (shouldStop(currentNode, stopRand)) {
            // Точка останавливается в этом узле
            path.back().color = Qt::red; // помечаем последний узел красным
            break;
        }
        
        // Выбираем следующий узел
        double moveRand = dist(rng);
        int nextNode = getNextNode(currentNode, moveRand);
        
        currentNode = nextNode;
        lastPath.push_back(currentNode);
        
        // Добавляем узел в путь
        path.push_back(PathPoint(currentNode, nodes[currentNode].level,
                                 nodeIdToName(currentNode), 
                                 nodes[currentNode].children.empty() ? Qt::green : Qt::blue));
    }
    
    lastPathLength = lastPath.size();
    return path;
}

void TreeModel::runMultipleSimulations(int numExperiments) {
    finalNodeIds.clear();
    pathLengths.clear();
    finalNodeIds.reserve(numExperiments);
    pathLengths.reserve(numExperiments);
    
    for (int exp = 0; exp < numExperiments; ++exp) {
        auto path = runSimulation();
        int lastNode = path.back().nodeId;
        finalNodeIds.push_back(lastNode);
        pathLengths.push_back(path.size());
    }
}

double TreeModel::getProbabilityToLeaf(int leafId) const {
    if (finalNodeIds.empty()) {
        return 0.0;
    }
    
    int count = std::count(finalNodeIds.begin(), finalNodeIds.end(), leafId);
    return static_cast<double>(count) / finalNodeIds.size();
}

double TreeModel::getProbabilityToPassVertices(int verticesCount) const {
    if (pathLengths.empty()) {
        return 0.0;
    }
    
    int count = std::count(pathLengths.begin(), pathLengths.end(), verticesCount);
    return static_cast<double>(count) / pathLengths.size();
}

QString TreeModel::getStatistics() const {
    if (finalNodeIds.empty()) {
        return "Нет данных. Запустите эксперименты.";
    }
    
    int totalExps = finalNodeIds.size();
    
    QString leafStats = "Вероятности попадания в листья:\n";
    for (int leafId : leafIds) {
        double prob = getProbabilityToLeaf(leafId);
        leafStats += QString("  Лист %1: %2%\n")
                        .arg(nodeIdToName(leafId))
                        .arg(prob * 100, 0, 'f', 2);
    }
    
    QString lengthStats = "\nРаспределение длин путей:\n";
    
    int maxLength = 0;
    for (int len : pathLengths) {
        if (len > maxLength) maxLength = len;
    }
    
    for (int len = 1; len <= maxLength; ++len) {
        double prob = getProbabilityToPassVertices(len);
        if (prob > 0) {
            lengthStats += QString("  Длина %1: %2%\n")
                              .arg(len)
                              .arg(prob * 100, 0, 'f', 2);
        }
    }

    double avgLength = 0.0;
    for (int len : pathLengths) {
        avgLength += len;
    }
    avgLength /= pathLengths.size();
    
    return leafStats + "\n" + lengthStats + 
           QString("\nСредняя длина пути: %1").arg(avgLength, 0, 'f', 2);
}
