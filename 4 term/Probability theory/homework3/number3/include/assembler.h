#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "quality_module.h"
#include <vector>
#include <functional>

struct Recipe {
    std::vector<int> componentIds;
    std::vector<int> quantities;
};

struct Product {
    int id;
    int quality;
    int timeProduced;
};

class Assembler {
public:
    Assembler(int id, int timeToAssemble, int moduleCount);
    
    bool canAssemble(const std::vector<int>& availableComponents) const;
    Product assemble(const std::vector<int>& components, int currentTime);
    void setRecipe(const Recipe& newRecipe);
    int getId() const;
    bool isBusy() const;
    void setBusy(bool busy);
    int getFinishTime() const;
    void setFinishTime(int time);
    double getSpeedMultiplier() const;

private:
    int id;
    int baseTime;
    int finishTime;
    bool busy;
    QualityModule qualityModule;
    Recipe recipe;
    
    int calculateQuality(const std::vector<int>& componentQualities, int currentTime);
};

#endif