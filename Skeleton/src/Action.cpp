#include "Action.h"
#include "Simulation.h"
#include "Settlement.h"
#include <iostream>
using namespace std;
extern Simulation *backup;

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
    return "SimulateStep";
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
    SelectionPolicy *policy = nullptr;
    if (selectionPolicy == "nve")
    {
        policy = new NaiveSelection();
    }
    else if (selectionPolicy == "bal")
    {
        policy = new BalancedSelection(0, 0, 0);
    }
    else if (selectionPolicy == "eco")
    {
        policy = new EconomySelection;
    }
    else if (selectionPolicy == "env")
    {
        policy = new EconomySelection;
    }
    simulation.addPlan(simulation.getSettlement(settlementName), policy);
    complete();
}

const string AddPlan::toString() const
{
    return "AddPlan";
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
    if (simulation.addSettlement(&Settlement(settlementName, settlementType)))
    {
        complete();
    }
    else
    {
        error("Settlement already exists");
    }
}

const string AddSettlement::toString() const
{
    return "AddSettlement";
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
    simulation.addFacility(Facility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
    complete();
}

const string AddFacility::toString() const
{
    return "AddFacility";
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
    return "PrintPlanStatus";
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
    Plan *plan = simulation.getPlan(planId);
    if (plan != nullptr)
    {
        plan->setSelectionPolicy(&newPolicy);
        complete();
    }
    else
    {
        error("Plan not found");
    }
}

const string ChangePlanPolicy::toString() const
{
    return "ChangePlanPolicy";
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
    return "PrintActionsLog";
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
    delete simulation;
    complete();
}

const string Close::toString() const
{
    return "Close";
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
    return "BackupSimulation";
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
    return "RestoreSimulation";
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}