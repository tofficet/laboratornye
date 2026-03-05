#ifndef RECYCLER_H
#define RECYCLER_H

#include "quality_module.h"
#include <vector>

struct RecycledComponents {
    std::vector<int> componentIds;
    std::vector<int> quantities;
    std::vector<int> qualities;
};

class Recycler {
public:
    Recycler(int id, int timeToRecycle, int moduleCount);
    
    RecycledComponents recycle(const Product& product, int currentTime);
    void setRecipe(const Recipe& recipe);
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
    
    int calculateQuality(int productQuality, int currentTime);
};

#endif