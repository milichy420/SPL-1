#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath);
    ~Simulation();                                      // Destructor
    Simulation(const Simulation &other);                // Copy constructor
    Simulation &operator=(const Simulation &other);     // Copy assignment operator
    Simulation(Simulation &&other) noexcept;            // Move constructor
    Simulation &operator=(Simulation &&other) noexcept; // Move assignment operator

    void start();
    void processCommand(const std::string &line);
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    vector<BaseAction *> getActionsLog();

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;

    void copyFrom(const Simulation &other);
    void moveFrom(Simulation &&other) noexcept;
};