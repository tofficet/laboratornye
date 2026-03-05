#include "quality_module.h"
#include <cmath>
#include <stdexcept>

QualityModule::QualityModule(int count) : moduleCount(count) {
    if (count < 0 || count > 4) {
        throw std::invalid_argument("Модулей качества должно быть от 0 до 4");
    }
}

int QualityModule::getCount() const {
    return moduleCount;
}

double QualityModule::getSpeedMultiplier() const {
    return 1.0 + moduleCount * 0.1;
}

int QualityModule::getMaxQualityIncrease() const {
    return moduleCount;
}

double QualityModule::getProbabilityForLevel(int increase) const {
    if (increase < 1 || increase > moduleCount) {
        return 0.0;
    }
    return BASE_PROB * pow(0.1, increase - 1);
}

int QualityModule::calculateResultQuality(int inputQuality, double randomValue) const {
    if (moduleCount == 0) {
        return inputQuality;
    }
    
    double cumulative = 0.0;
    
    for (int inc = 1; inc <= moduleCount; ++inc) {
        cumulative += getProbabilityForLevel(inc);
        if (randomValue < cumulative) {
            return std::min(inputQuality + inc, 5);
        }
    }
    
    return inputQuality;
}