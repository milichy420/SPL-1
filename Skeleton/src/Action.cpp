#include "Action.h"
#include "Simulation.h"
#include "Settlement.h"
#include "Facility.h"
#include "Auxiliary.h"
#include <iostream>
#include <algorithm>
using namespace std;
extern Simulation *backup;

// ActionStatus toString function
std::string actionStatusToString(ActionStatus status)
{
    switch (status)
    {
    case ActionStatus::COMPLETED:
        return "COMPLETED";
    case ActionStatus::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

// FacilityCategory toString function
std::string facilityCategoryToString(FacilityCategory category)
{
    switch (category)
    {
    case FacilityCategory::LIFE_QUALITY:
        return "LIFE_QUALITY";
    case FacilityCategory::ECONOMY:
        return "ECONOMY";
    case FacilityCategory::ENVIRONMENT:
        return "ENVIRONMENT";
    default:
        return "UNKNOWN";
    }
}

// BaseAction implementation
BaseAction::BaseAction() : status(ActionStatus::ERROR), errorMsg("") {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    std::cerr << "Error: " << errorMsg << std::endl;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

// SimulateStep implementation
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; ++i)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    return "step " + std::to_string(numOfSteps) + " " + actionStatusToString(getStatus());
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

// AddPlan implementation
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy *policy;
    try
    {
        policy = Auxiliary::createSelectionPolicy(selectionPolicy);
    }
    catch (std::runtime_error)
    {
        error("Cannot create this plan, Selection Policy doesn't exist");
    }
    try
    {
        simulation.addPlan(simulation.getSettlement(settlementName), policy);
        complete();
    }
    catch (std::runtime_error)
    {
        error("Cannot create this plan, Settlement doesn't exist");
    }
}

const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + selectionPolicy + " " + actionStatusToString(getStatus());
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

// AddSettlement implementation
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    Settlement *settlement = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(settlement))
    {
        complete();
    }
    else
    {
        delete settlement;
        error("Settlement already exists");
    }
}

const string AddSettlement::toString() const
{
    return "settlement " + settlementName + " " + std::to_string(static_cast<unsigned int>(settlementType) - 1) + " " + actionStatusToString(getStatus());
    // the settlement numbers correlate to the building limit and as such are 1 higher, so we reduce them by 1
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

// AddFacility implementation
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation)
{
    try
    {
        simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
        complete();
    }
    catch (const std::exception &e)
    {
        error(e.what());
    }
}

const string AddFacility::toString() const
{
    return "facility " + facilityName + " " + facilityCategoryToString(facilityCategory) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + actionStatusToString(getStatus());
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

// PrintPlanStatus implementation
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation)
{
    try
    {
        Plan &plan = simulation.getPlan(planId);
        plan.printStatus();
        complete();
    }
    catch (std::runtime_error)
    {
        error("Plan doesn't exist");
    }
}

const string PrintPlanStatus::toString() const
{
    return "planStatus " + std::to_string(planId) + " " + actionStatusToString(getStatus());
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

// ChangePlanPolicy implementation
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation)
{
    // should error when the previous policy is the same as the new policy or if the planID doesn't exist
    try
    {
        Plan &plan = simulation.getPlan(planId);
        if (newPolicy != plan.getSelectionPolicy()->toString())
        {
            SelectionPolicy *policy = Auxiliary::createSelectionPolicy(newPolicy);
            plan.setSelectionPolicy(policy);
            complete();
        }
        else
        {
            error("Cannot change selection policy");
        }
    }
    catch (const std::runtime_error &e)
    {
        error("Cannot change selection policy");
    }
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + actionStatusToString(getStatus());
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

// PrintActionsLog implementation
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation)
{
    const vector<BaseAction *> &actionsLog = simulation.getActionsLog();
    for (const auto &action : actionsLog)
    {
        cout << action->toString() << endl;
    }
    complete();
}

const string PrintActionsLog::toString() const
{
    return "log " + actionStatusToString(getStatus());
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

// Close implementation
Close::Close() {}

void Close::act(Simulation &simulation)
{
    simulation.close();
    complete();
}

const string Close::toString() const
{
    return "close " + actionStatusToString(getStatus());
}

Close *Close::clone() const
{
    return new Close(*this);
}

// BackupSimulation implementation
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation)
{
    if (backup != nullptr)
    {
        delete backup;
    }
    backup = new Simulation(simulation); // Use the copy constructor
    complete();
}

const string BackupSimulation::toString() const
{
    return "backup " + actionStatusToString(getStatus());
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

// RestoreSimulation implementation
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation)
{
    if (backup != nullptr)
    {
        simulation = *backup; // Use the assignment operator
        complete();
    }
    else
    {
        error("No backup available");
    }
}

const string RestoreSimulation::toString() const
{
    return "restore " + actionStatusToString(getStatus());
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}