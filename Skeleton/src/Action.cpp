#include "Action.h"

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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
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
    // Implementation of act
}

const string RestoreSimulation::toString() const
{
    return "RestoreSimulation";
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}