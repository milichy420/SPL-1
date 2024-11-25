#include "Simulation.h"
#include "BaseAction.h"
#include "SelectionPolicy.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0)
{
    // Load configuration from file
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Could not open config file");
    }

    // Parse the config file and initialize facilitiesOptions, settlements, etc.
    // This is a placeholder for actual parsing logic
    // ...

    configFile.close();
}

void Simulation::start()
{
    isRunning = true;
    std::cout << "Simulation started" << std::endl;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.emplace_back(planCounter++, settlement, selectionPolicy);
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (isSettlementExists(settlement->getName()))
    {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (const auto &settlement : settlements)
    {
        if (settlement.getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (auto &settlement : settlements)
    {
        if (settlement.getName() == settlementName)
        {
            return settlement;
        }
    }
    throw std::runtime_error("Settlement not found");
}

Plan &Simulation::getPlan(const int planID)
{
    for (auto &plan : plans)
    {
        if (plan.getID() == planID)
        {
            return plan;
        }
    }
    throw std::runtime_error("Plan not found");
}

void Simulation::step()
{
    if (!isRunning)
    {
        throw std::runtime_error("Simulation is not running");
    }

    for (auto &plan : plans)
    {
        plan.step();
    }
}

void Simulation::close()
{
    isRunning = false;
    std::cout << "Simulation closed" << std::endl;
}

void Simulation::open()
{
    isRunning = true;
    std::cout << "Simulation opened" << std::endl;
}