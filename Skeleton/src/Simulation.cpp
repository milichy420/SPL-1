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
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement settlement)
{
    if (isSettlementExists(settlement.getName()))
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
    // Execute a single step of the simulation
    // This is a placeholder for actual step logic
    // ...

    for (auto &plan : plans)
    {
        if (plan.getStatus() == PlanStatus::AVALIABLE)
        {
            for (int i = 0; i < plan.getSettlement().getType(); ++i)
            {
                // i am not sure what to do here: what is plan.step()? what is the other auto generated methods?

                plan.step();

                FacilityType nextFacility = plan.getSelectionPolicy()->selectFacility(facilitiesOptions);
                plan.addFacility(nextFacility);
            }
            /* use selection policy to choose the next facility to build. */
        }
        for (auto &facility : plan.getUnderConstruction())
        {
            facility.step();
            if (facility.getStatus() == FacilityStatus::OPERATIONAL)
            {
                plan.moveFacilityToOperational(facility);
            }
        }
        if (plan.getUnderConstruction().length() == plan.getSettlement().getType())
        {
            plan.setStatus(PlanStatus::BUSY);
        }
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