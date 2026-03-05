#ifndef FACTORY_H
#define FACTORY_H

#include "assembler.h"
#include "recycler.h"
#include <vector>
#include <memory>
#include <queue>

struct ComponentStock {
    int id;
    int quantity;
    int quality;
};

class Factory {
public:
    Factory(int numAssemblers, int assemblerTime, int assemblerModules,
            int numRecyclers, int recyclerTime, int recyclerModules,
            const std::vector<int>& supplyRates,
            const Recipe& productionRecipe);
    
    void runSimulation();
    const std::vector<int>& getLegendaryTimes() const;
    const std::vector<int>& getEpicTimes() const;
    const std::vector<int>& getRareTimes() const;

private:
    int currentTime;
    int legendaryCount;
    
    std::vector<std::unique_ptr<Assembler>> assemblers;
    std::vector<std::unique_ptr<Recycler>> recyclers;
    std::vector<ComponentStock> warehouse;
    std::vector<int> supplyRates;
    Recipe recipe;
    
    std::vector<int> legendaryTimes;
    std::vector<int> epicTimes;
    std::vector<int> rareTimes;
    
    struct Event {
        int time;
        enum Type { ASSEMBLER_FINISH, RECYCLER_FINISH, SUPPLY_ARRIVAL } type;
        int machineId;
        
        bool operator>(const Event& other) const {
            return time > other.time;
        }
    };
    
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> eventQueue;
    
    void processSupplyEvent();
    void processAssemblerFinish(int assemblerId);
    void processRecyclerFinish(int recyclerId);
    void checkAndStartProduction();
    void addToWarehouse(int componentId, int quantity, int quality);
    bool hasEnoughComponents(const std::vector<int>& needed) const;
    std::vector<int> takeComponents(const std::vector<int>& needed);
    void recordProduct(int quality, int time);
};

#endif