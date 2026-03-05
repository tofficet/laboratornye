#include "assembler.h"
#include <random>
#include <algorithm>

Assembler::Assembler(int id, int timeToAssemble, int moduleCount)
    : id(id), baseTime(timeToAssemble), finishTime(0), busy(false), qualityModule(moduleCount) {
}

void Assembler::setRecipe(const Recipe& newRecipe) {
    recipe = newRecipe;
}

bool Assembler::canAssemble(const std::vector<int>& availableComponents) const {
    for (size_t i = 0; i < recipe.componentIds.size(); ++i) {
        int compId = recipe.componentIds[i];
        int needed = recipe.quantities[i];
        
        if (compId >= static_cast<int>(availableComponents.size()) || 
            availableComponents[compId] < needed) {
            return false;
        }
    }
    return true;
}

Product Assembler::assemble(const std::vector<int>& components, int currentTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    std::vector<int> componentQualities;
    
    for (size_t i = 0; i < recipe.componentIds.size(); ++i) {
        int compId = recipe.componentIds[i];
        int needed = recipe.quantities[i];
    }
    
    int resultQuality = calculateQuality(componentQualities, currentTime);
    
    int assemblyTime = static_cast<int>(baseTime * qualityModule.getSpeedMultiplier());
    finishTime = currentTime + assemblyTime;
    busy = true;
    
    Product p;
    p.id = id;
    p.quality = resultQuality;
    p.timeProduced = finishTime;
    
    return p;
}

int Assembler::calculateQuality(const std::vector<int>& componentQualities, int currentTime) {
    int minQuality = 5;
    for (int q : componentQualities) {
        minQuality = std::min(minQuality, q);
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    double randVal = dist(gen);
    return qualityModule.calculateResultQuality(minQuality, randVal);
}

int Assembler::getId() const {
    return id;
}

bool Assembler::isBusy() const {
    return busy;
}

void Assembler::setBusy(bool b) {
    busy = b;
}

int Assembler::getFinishTime() const {
    return finishTime;
}

void Assembler::setFinishTime(int time) {
    finishTime = time;
}

double Assembler::getSpeedMultiplier() const {
    return qualityModule.getSpeedMultiplier();
}   