#include "factory.h"
#include <algorithm>
#include <iostream>

Factory::Factory(int numAssemblers, int assemblerTime, int assemblerModules,
                 int numRecyclers, int recyclerTime, int recyclerModules,
                 const std::vector<int>& supplyRates,
                 const Recipe& productionRecipe)
    : currentTime(0), legendaryCount(0), supplyRates(supplyRates), recipe(productionRecipe) {
    
    for (int i = 0; i < numAssemblers; ++i) {
        assemblers.push_back(std::make_unique<Assembler>(i, assemblerTime, assemblerModules));
        assemblers.back()->setRecipe(productionRecipe);
    }
    
    for (int i = 0; i < numRecyclers; ++i) {
        recyclers.push_back(std::make_unique<Recycler>(i, recyclerTime, recyclerModules));
        recyclers.back()->setRecipe(productionRecipe);
    }
    
    for (size_t i = 0; i < productionRecipe.componentIds.size(); ++i) {
        ComponentStock stock;
        stock.id = productionRecipe.componentIds[i];
        stock.quantity = 0;
        stock.quality = 1;
        warehouse.push_back(stock);
    }
    
    for (int time = 1; time <= 1000; ++time) {
        Event e;
        e.time = time;
        e.type = Event::SUPPLY_ARRIVAL;
        e.machineId = -1;
        eventQueue.push(e);
    }
}

void Factory::runSimulation() {
    while (legendaryCount < 25 && !eventQueue.empty()) {
        Event nextEvent = eventQueue.top();
        eventQueue.pop();
        
        currentTime = nextEvent.time;
        
        switch (nextEvent.type) {
            case Event::SUPPLY_ARRIVAL:
                processSupplyEvent();
                break;
            case Event::ASSEMBLER_FINISH:
                processAssemblerFinish(nextEvent.machineId);
                break;
            case Event::RECYCLER_FINISH:
                processRecyclerFinish(nextEvent.machineId);
                break;
        }
        
        checkAndStartProduction();
    }
}

void Factory::processSupplyEvent() {
    for (size_t i = 0; i < supplyRates.size(); ++i) {
        warehouse[i].quantity += supplyRates[i];
    }
}

void Factory::processAssemblerFinish(int assemblerId) {
    assemblers[assemblerId]->setBusy(false);
}

void Factory::processRecyclerFinish(int recyclerId) {
    recyclers[recyclerId]->setBusy(false);
}

void Factory::checkAndStartProduction() {
    for (auto& assembler : assemblers) {
        if (!assembler->isBusy() && assembler->canAssemble(getComponentQuantities())) {
            auto components = takeComponents(recipe.quantities);
            Product p = assembler->assemble(components, currentTime);
            
            Event e;
            e.time = p.timeProduced;
            e.type = Event::ASSEMBLER_FINISH;
            e.machineId = assembler->getId();
            eventQueue.push(e);
            
            recordProduct(p.quality, p.timeProduced);
            
            if (p.quality == 5) {
                legendaryCount++;
            }
        }
    }
}

std::vector<int> Factory::getComponentQuantities() const {
    std::vector<int> quantities;
    for (const auto& stock : warehouse) {
        quantities.push_back(stock.quantity);
    }
    return quantities;
}

bool Factory::hasEnoughComponents(const std::vector<int>& needed) const {
    for (size_t i = 0; i < needed.size(); ++i) {
        if (warehouse[i].quantity < needed[i]) {
            return false;
        }
    }
    return true;
}

std::vector<int> Factory::takeComponents(const std::vector<int>& needed) {
    std::vector<int> taken;
    for (size_t i = 0; i < needed.size(); ++i) {
        warehouse[i].quantity -= needed[i];
        taken.push_back(warehouse[i].quality);
    }
    return taken;
}

void Factory::addToWarehouse(int componentId, int quantity, int quality) {
    for (auto& stock : warehouse) {
        if (stock.id == componentId) {
            stock.quantity += quantity;
            stock.quality = std::max(stock.quality, quality);
            break;
        }
    }
}

void Factory::recordProduct(int quality, int time) {
    switch (quality) {
        case 3:
            rareTimes.push_back(time);
            break;
        case 4:
            epicTimes.push_back(time);
            break;
        case 5:
            legendaryTimes.push_back(time);
            break;
    }
}

const std::vector<int>& Factory::getLegendaryTimes() const {
    return legendaryTimes;
}

const std::vector<int>& Factory::getEpicTimes() const {
    return epicTimes;
}

const std::vector<int>& Factory::getRareTimes() const {
    return rareTimes;
}