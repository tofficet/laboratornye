#include "recycler.h"
#include <random>

Recycler::Recycler(int id, int timeToRecycle, int moduleCount)
    : id(id), baseTime(timeToRecycle), finishTime(0), busy(false), qualityModule(moduleCount) {
}

void Recycler::setRecipe(const Recipe& recipe) {
    this->recipe = recipe;
}

RecycledComponents Recycler::recycle(const Product& product, int currentTime) {
    RecycledComponents result;
    
    double returnRate = 0.25;
    int resultQuality = calculateQuality(product.quality, currentTime);
    
    for (size_t i = 0; i < recipe.componentIds.size(); ++i) {
        int compId = recipe.componentIds[i];
        int quantity = recipe.quantities[i];
        
        int returnedQuantity = static_cast<int>(quantity * returnRate);
        if (returnedQuantity < 1 && quantity > 0) {
            returnedQuantity = 1;
        }
        
        result.componentIds.push_back(compId);
        result.quantities.push_back(returnedQuantity);
        result.qualities.push_back(resultQuality);
    }
    
    int recycleTime = static_cast<int>(baseTime * qualityModule.getSpeedMultiplier());
    finishTime = currentTime + recycleTime;
    busy = true;
    
    return result;
}

int Recycler::calculateQuality(int productQuality, int currentTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    double randVal = dist(gen);
    return qualityModule.calculateResultQuality(productQuality, randVal);
}

int Recycler::getId() const {
    return id;
}

bool Recycler::isBusy() const {
    return busy;
}

void Recycler::setBusy(bool b) {
    busy = b;
}

int Recycler::getFinishTime() const {
    return finishTime;
}

void Recycler::setFinishTime(int time) {
    finishTime = time;
}

double Recycler::getSpeedMultiplier() const {
    return qualityModule.getSpeedMultiplier();
}