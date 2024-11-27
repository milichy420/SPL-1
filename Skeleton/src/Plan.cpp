#include "Plan.h"
#include <iostream>
using namespace std;
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE), life_quality_score(0), economy_score(0), environment_score(0) {}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}

void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        while (underConstruction.size() <= settlement.getType())
        {
            FacilityType nextFacility = selectionPolicy->selectFacility(facilitiesOptions);
            underConstruction.push_back(nextFacility);
        }
    }

    for (auto &facility : underConstruction)
    {
        facility.step();
        if (facility.getStatus() == FacilityStatus::OPERATIONAL)
        {
            moveFacilityToOperational(facility);
        }
    }
    if (underConstruction.size() >= settlement.getType())
    {
        plan.setStatus(PlanStatus::BUSY);
    }
    else
    {
        plan.setStatus(PlanStatus::AVALIABLE);
    }
}

void Plan::printStatus()
{
    cout << "PlanID: " << plan_id << std::endl;
    cout << "SettlementName: " << settlement.getName() << std::endl;
    cout << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "AVAILABLE" : "BUSY") << std::endl;
    cout << "SelectionPolicy: " << selectionPolicy->toString() << std::endl;
    cout << "LifeQualityScore: " << life_quality_score << std::endl;
    cout << "EconomyScore: " << economy_score << std::endl;
    cout << "EnvironmentScore: " << environment_score << std::endl;
    for(Facility *facility : facilities){
        cout << "FacilityName: " << facility->getName() << endl;
        cout << "FacilityStatus: OPERATIONAL" << endl;
    }
    for(Facility *facility : underConstruction){
        cout << "FacilityName: " << facility->getName() << endl;
        cout << "FacilityStatus: UNDER_CONSTRUCTIONS" << endl;
    }
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
    // Update scores based on the new facility
}

const string Plan::toString() const
{
    // Implement a string representation of the plan
    return "Plan ID: " + std::to_string(plan_id);
}

const Settlement Plan::getSettlement() const
{
    return settlement;
}

void Plan::moveFacilityToUnderConstruction(Facility *facility)
{
    underConstruction.add(facility);
    facilities.erase(
        remove(facilities.begin(), facilities.end(), facility),
        facilities.end());

    // Move a facility from operational to under construction
    // Update scores accordingly
}

void Plan::moveFacilityToOperational(Facility *facility)
{
    facilities.add(facility);
    underConstruction.erase(
        remove(underConstruction.begin(), underConstruction.end(), facility),
        underConstruction.end());
    life_quality_score += facility->getLifeQualityScore();
    economy_score += facility->getEconomyScore();
    environment_score += facility->getEnvironmentScore();

    // Move a facility from under construction to operational
    // Update scores accordingly
}