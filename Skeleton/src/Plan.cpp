#include "Plan.h"
#include <iostream>
using namespace std;
#include <utility> // For std::move

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE), life_quality_score(0), economy_score(0), environment_score(0) {}

// Destructor
Plan::~Plan()
{
    delete selectionPolicy;

    for (auto facility : facilities)
    {
        delete facility;
    }
    facilities.clear();
    for (auto facility : underConstruction)
    {
        delete facility;
    }
    underConstruction.clear();
}

// Copy constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id), settlement(other.settlement), facilityOptions(other.facilityOptions)
{
    copyFrom(other);
}

// Copy counstructor 2
Plan::Plan(const Plan &other, const Settlement &settlement, const vector<FacilityType> &facilityOptions)
    : plan_id(other.plan_id), settlement(settlement), facilityOptions(facilityOptions)
{
    copyFrom(other);
}

// Copy assignment operator
Plan &Plan::operator=(const Plan &other)
{
    if (this != &other)
    {
        // Clean up existing resources
        delete selectionPolicy;
        for (auto facility : facilities)
        {
            delete facility;
        }
        facilities.clear();
        for (auto facility : underConstruction)
        {
            delete facility;
        }
        underConstruction.clear();
        // Copy from other
        copyFrom(other);
    }
    return *this;
}

// Move constructor
Plan::Plan(Plan &&other) noexcept
    : plan_id(other.plan_id), settlement(other.settlement), facilityOptions(other.facilityOptions)
{
    moveFrom(std::move(other));
}

// Move assignment operator
Plan &Plan::operator=(Plan &&other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resources
        delete selectionPolicy;

        for (auto facility : facilities)
        {
            delete facility;
        }
        facilities.clear();
        for (auto facility : underConstruction)
        {
            delete facility;
        }
        underConstruction.clear();
        // Move from other
        moveFrom(std::move(other));
    }
    return *this;
}

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

SelectionPolicy *Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}

void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        while (underConstruction.size() < static_cast<unsigned int>(settlement.getType()))
        {
            FacilityType nextFacilityType = selectionPolicy->selectFacility(facilityOptions);
            Facility *nextFacility = new Facility(nextFacilityType, settlement.getName());
            underConstruction.push_back(nextFacility);
        }
    }

    for (auto it = underConstruction.begin(); it != underConstruction.end();)
    {
        (*it)->step();
        if ((*it)->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(*it);
            it = underConstruction.erase(it);

            life_quality_score += (*it)->getLifeQualityScore();
            economy_score += (*it)->getEconomyScore();
            environment_score += (*it)->getEnvironmentScore();
        }
        else
        {
            ++it;
        }
    }
    if (underConstruction.size() >= static_cast<unsigned int>(settlement.getType()))
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
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
    for (Facility *facility : facilities)
    {
        cout << "FacilityName: " << facility->getName() << endl;
        cout << "FacilityStatus: OPERATIONAL" << endl;
    }
    for (Facility *facility : underConstruction)
    {
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
}

const string Plan::toString() const
{
    // For the close operation in the simulation
    return "PlanID: " + std::to_string(plan_id) + "\n" +
           "SettlementName: " + settlement.getName() + "\n" +
           "LifeQuality_Score: " + std::to_string(life_quality_score) + "\n" +
           "Economy_Score: " + std::to_string(economy_score) + "\n" +
           "Environment_Score: " + std::to_string(environment_score);
}

const int Plan::getId() const
{
    return plan_id;
}

const Settlement Plan::getSettlement() const
{
    return settlement;
}

void Plan::copyFrom(const Plan &other)
{
    selectionPolicy = other.selectionPolicy->clone();
    status = other.status;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;
    for (const auto facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }
    for (const auto facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

void Plan::moveFrom(Plan &&other) noexcept
{
    selectionPolicy = other.selectionPolicy;
    status = other.status;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;
    facilities = std::move(other.facilities);
    underConstruction = std::move(other.underConstruction);

    // Reset the other plan
    other.selectionPolicy = nullptr;
    other.status = PlanStatus::AVALIABLE;
    other.life_quality_score = 0;
    other.economy_score = 0;
    other.environment_score = 0;
    other.facilities.clear();
    other.underConstruction.clear();
}