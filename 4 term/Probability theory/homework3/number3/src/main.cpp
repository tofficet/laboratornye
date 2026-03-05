#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "factory.h"

int main(int argc, char* argv[]) {
    if (argc < 8) {
        std::cerr << "Использование: " << argv[0] 
                  << " <AM> <RM> <t_сб> <t_пр> <рецепт> <скорости_поступления>" 
                  << std::endl;
        std::cerr << "Пример: " << argv[0] << " 2 1 5 3 \"2,3,1\" \"5,3,2\"" 
                  << std::endl;
        return 1;
    }
    
    int AM = std::atoi(argv[1]);
    int RM = std::atoi(argv[2]);
    int t_sb = std::atoi(argv[3]);
    int t_pr = std::atoi(argv[4]);
    
    std::string recipeStr = argv[5];
    std::string supplyStr = argv[6];
    
    Recipe recipe;
    std::vector<int> supplyRates;
    
    size_t pos = 0;
    while ((pos = recipeStr.find(',')) != std::string::npos) {
        recipe.quantities.push_back(std::stoi(recipeStr.substr(0, pos)));
        recipeStr.erase(0, pos + 1);
    }
    recipe.quantities.push_back(std::stoi(recipeStr));
    
    for (int i = 0; i < static_cast<int>(recipe.quantities.size()); ++i) {
        recipe.componentIds.push_back(i);
    }
    
    pos = 0;
    while ((pos = supplyStr.find(',')) != std::string::npos) {
        supplyRates.push_back(std::stoi(supplyStr.substr(0, pos)));
        supplyStr.erase(0, pos + 1);
    }
    supplyRates.push_back(std::stoi(supplyStr));
    
    Factory factory(AM, t_sb, 0, RM, t_pr, 0, supplyRates, recipe);
    
    factory.runSimulation();
    
    std::cout << "Время производства легендарных продуктов (L5):" << std::endl;
    for (int time : factory.getLegendaryTimes()) {
        std::cout << time << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Время производства эпических продуктов (L4):" << std::endl;
    for (int time : factory.getEpicTimes()) {
        std::cout << time << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Время производства редких продуктов (L3):" << std::endl;
    for (int time : factory.getRareTimes()) {
        std::cout << time << " ";
    }
    std::cout << std::endl;
    
    return 0;
}