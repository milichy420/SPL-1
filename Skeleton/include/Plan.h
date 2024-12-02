#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};

class Plan
{
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    ~Plan();                                // Destructor
    Plan(const Plan &other);                // Copy constructor
    Plan &operator=(const Plan &other);     // Copy assignment operator
    Plan(Plan &&other) noexcept;            // Move constructor
    Plan &operator=(Plan &&other) noexcept; // Move assignment operator

    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    SelectionPolicy *getSelectionPolicy() const;
    void step();
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    void addFacility(Facility *facility);
    const string toString() const;
    const int getId() const;

    const Settlement getSettlement() const;
    void moveFacilityToUnderConstruction(Facility *facility);
    void moveFacilityToOperational(Facility *facility);

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;

    void copyFrom(const Plan &other);
    void moveFrom(Plan &&other) noexcept;
};