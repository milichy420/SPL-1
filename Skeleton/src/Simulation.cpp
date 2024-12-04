#include "Simulation.h"
#include "Action.h"
#include "SelectionPolicy.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Auxiliary.h"
#include <utility>

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0)
{
    // Load configuration from file
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Could not open config file");
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        std::vector<std::string> parsedArguments = Auxiliary::parseArguments(line);
        if (!parsedArguments.empty())
        {
            const std::string &command = parsedArguments[0];
            if (command == "settlement")
            {
                if (parsedArguments.size() >= 3)
                {
                    std::string name = parsedArguments[1];
                    SettlementType type = Auxiliary::parseSettlementType(parsedArguments[2]);

                    Settlement *settlement = new Settlement(name, type);
                    if (!addSettlement(settlement))
                    {
                        delete settlement;
                        throw std::runtime_error("Settlement already exists");
                    }
                }
            }
            else if (command == "facility")
            {
                if (parsedArguments.size() >= 7)
                {
                    std::string name = parsedArguments[1];
                    FacilityCategory category = Auxiliary::parseFacilityCategory(parsedArguments[2]);

                    int price = std::stoi(parsedArguments[3]);
                    int lifeQualityImpact = std::stoi(parsedArguments[4]);
                    int ecoImpact = std::stoi(parsedArguments[5]);
                    int envImpact = std::stoi(parsedArguments[6]);
                    FacilityType facility(name, category, price, lifeQualityImpact, ecoImpact, envImpact);
                    addFacility(facility);
                }
            }
            else if (command == "plan")
            {
                if (parsedArguments.size() >= 3)
                {
                    Settlement &settlement = getSettlement(parsedArguments[1]);
                    SelectionPolicy *selectionPolicy = Auxiliary::createSelectionPolicy(parsedArguments[2]);
                    addPlan(settlement, selectionPolicy);
                }
            }
        }
    }

    configFile.close();
}

// Destructor
Simulation::~Simulation()
{
    for (auto action : actionsLog)
    {
        delete action;
    }
    for (auto settlement : settlements)
    {
        delete settlement;
    }
}

// Copy constructor
Simulation::Simulation(const Simulation &other)
{
    copyFrom(other);
}

// Copy assignment operator
Simulation &Simulation::operator=(const Simulation &other)
{
    if (this != &other)
    {
        // Clean up existing resources
        for (auto action : actionsLog)
        {
            delete action;
        }
        for (auto settlement : settlements)
        {
            delete settlement;
        }
        // Copy from other
        copyFrom(other);
    }
    return *this;
}

// Move constructor
Simulation::Simulation(Simulation &&other) noexcept
{
    moveFrom(std::move(other));
}

// Move assignment operator
Simulation &Simulation::operator=(Simulation &&other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resources
        for (auto action : actionsLog)
        {
            delete action;
        }
        for (auto settlement : settlements)
        {
            delete settlement;
        }
        // Move from other
        moveFrom(std::move(other));
    }
    return *this;
}

void Simulation::start()
{
    isRunning = true;
    std::cout << "The simulation has started" << std::endl;

    while (isRunning)
    {
        std::string line;
        std::cout << "Enter a command: ";
        std::getline(std::cin, line);

        processCommand(line);
    }
}

void Simulation::processCommand(const std::string &line)
{
    std::vector<std::string> parsedArguments = Auxiliary::parseArguments(line);
    if (!parsedArguments.empty())
    {
        const std::string &command = parsedArguments[0];
        if (command == "step")
        {
            BaseAction *action = new SimulateStep(std::stoi(parsedArguments[1]));
            executeAction(action);
        }
        else if (command == "plan")
        {
            BaseAction *action = new AddPlan(parsedArguments[1], parsedArguments[2]);
            executeAction(action);
        }
        else if (command == "settlement")
        {
            SettlementType type = Auxiliary::parseSettlementType(parsedArguments[2]);
            BaseAction *action = new AddSettlement(parsedArguments[1], type);
            executeAction(action);
        }
        else if (command == "facility")
        {
            FacilityCategory category = Auxiliary::parseFacilityCategory(parsedArguments[2]);
            BaseAction *action = new AddFacility(parsedArguments[1], category, std::stoi(parsedArguments[3]), std::stoi(parsedArguments[4]), std::stoi(parsedArguments[5]), std::stoi(parsedArguments[6]));
            executeAction(action);
        }
        else if (command == "planStatus")
        {
            BaseAction *action = new PrintPlanStatus(std::stoi(parsedArguments[1]));
            executeAction(action);
        }
        else if (command == "changePolicy")
        {
            BaseAction *action = new ChangePlanPolicy(std::stoi(parsedArguments[1]), parsedArguments[2]);
            executeAction(action);
        }
        else if (command == "log")
        {
            BaseAction *action = new PrintActionsLog();
            executeAction(action);
        }
        else if (command == "close")
        {
            BaseAction *action = new Close();
            executeAction(action);
        }
        else if (command == "backup")
        {
            BaseAction *action = new BackupSimulation();
            executeAction(action);
        }
        else if (command == "restore")
        {
            BaseAction *action = new RestoreSimulation();
            executeAction(action);
        }
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}

void Simulation::executeAction(BaseAction *action)
{
    action->act(*this);
    addAction(action);
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    // Create a plan with the given settlement and selection policy and add it to the plans vector.
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions);
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
    for(FacilityType current_facility : facilitiesOptions){
        if(current_facility.getName() == facility.getName()){
            throw std::runtime_error("facility already exists");
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (const auto &settlement : settlements)
    {
        if (settlement->getName() == settlementName)
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
        if (settlement->getName() == settlementName)
        {
            return *settlement;
        }
    }
    throw std::runtime_error("Settlement not found");
}

Plan &Simulation::getPlan(const int planID)
{
    for (auto &plan : plans)
    {
        if (plan.getId() == planID)
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
        std::cout << "Facility options pointer: " << &facilitiesOptions << std::endl;
        plan.step();
    }
}

void Simulation::close()
{
    for (const auto &plan : plans)
    {
        std::cout << plan.toString() << std::endl;
    }
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}

vector<BaseAction *> Simulation::getActionsLog()
{
    return actionsLog;
}

void Simulation::copyFrom(const Simulation &other)
{
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    for (const auto action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
    for (const auto settlement : other.settlements)
    {
        settlements.push_back(new Settlement(*settlement));
    }
    plans = other.plans;

    facilitiesOptions.clear();
    for (FacilityType facility : other.facilitiesOptions)
    {
        facilitiesOptions.push_back(facility);
    }
}

void Simulation::moveFrom(Simulation &&other) noexcept
{
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = std::move(other.actionsLog);
    settlements = std::move(other.settlements);
    plans = std::move(other.plans);
    facilitiesOptions = std::move(other.facilitiesOptions);

    // Reset the other simulation
    other.isRunning = false;
    other.planCounter = 0;
    other.actionsLog.clear();
    other.settlements.clear();
    other.plans.clear();
    other.facilitiesOptions.clear();
}
