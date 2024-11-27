#include "SelectionPolicy.h"
#include <stdexcept>

// NaiveSelection implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }
    lastSelectedIndex++;
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

// BalancedSelection implementation
BalancedSelection::BalancedSelection(int lifeQualityScore, int economyScore, int environmentScore)
    : LifeQualityScore(lifeQualityScore), EconomyScore(economyScore), EnvironmentScore(environmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }

    int minDistance = std::numeric_limits<int>::max();
    size_t selectedIndex = 0;

    for (size_t i = 0; i < facilitiesOptions.size(); ++i)
    {
        int life = facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore;
        int eco = facilitiesOptions[i].getEconomyScore() + EconomyScore;
        int env = facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore;
        int distance = std::max({std::abs(life - eco), std::abs(life - env), std::abs(eco - env)});

        if (distance < minDistance)
        {
            minDistance = distance;
            selectedIndex = i;
        }
    }

    return facilitiesOptions[selectedIndex];
}

const string BalancedSelection::toString() const
{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

// EconomySelection implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }

    do
    {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    } while (facilitiesOptions[lastSelectedIndex].getCategory() != "economy");

    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

// SustainabilitySelection implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }
    do
    {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    } while (facilitiesOptions[lastSelectedIndex].getCategory() != "environment");

    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}