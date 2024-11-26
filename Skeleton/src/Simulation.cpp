#include "Simulation.h"
#include "BaseAction.h"
#include "SelectionPolicy.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Auxiliary.h"

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
        // Process each line of the config file
        std::cout << "Read line: " << line << std::endl;

        processCommand(line);
    }

    configFile.close();
}

void Simulation::start()
{
    isRunning = true;
    std::cout << "The simulation has started" << std::endl;

    while (isRunning)
    {
        std::string line;
        std::cout << "Enter a command: ";
        std::cin >> line;
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
            const int num_of_steps = std::stoi(parsedArguments[1]);
            for (int i = 0; i < num_of_steps; ++i)
            {
                step();
            }
        }
        else if (command == "plan")
        {
            if (parsedArguments.size() >= 2)
            {
                FacilityType facility = Auxiliary::stringToFacilityType(parsedArguments[1]);
                addFacility(facility);
            }
        }
        else if (command == "settlement")
        {
            if (parsedArguments.size() >= 3)
            {
                Settlement &settlement = getSettlement(parsedArguments[1]);
                SelectionPolicy *selectionPolicy = Auxiliary::createSelectionPolicy(parsedArguments[2]);
                addPlan(settlement, selectionPolicy);
            }
        }
        else if (command == "facility")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "planStatus")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "changePolicy")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "log")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "close")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "backup")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else if (command == "restore")
        {
            if (parsedArguments.size() >= 2)
            {
                BaseAction *action = Auxiliary::createAction(parsedArguments[1]);
                addAction(action);
            }
        }
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    // Create a plan with the given settlement and selection policy and add it to the plans vector.
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilityOptions);
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

vector<BaseAction *> Simulation::getActionsLog()
{
    return actionsLog;
}