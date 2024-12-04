#include "Auxiliary.h"
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string &line)
{
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument)
    {
        arguments.push_back(argument);
    }

    return arguments;
}

SettlementType Auxiliary::parseSettlementType(const std::string &type)
{
    if (type == "0")
    {
        return SettlementType::VILLAGE;
    }
    else if (type == "1")
    {
        return SettlementType::CITY;
    }
    else if (type == "2")
    {
        return SettlementType::METROPOLIS;
    }
    else
    {
        throw std::invalid_argument("Invalid settlement type");
    }
}

FacilityCategory Auxiliary::parseFacilityCategory(const std::string &category)
{
    if (category == "0")
    {
        return FacilityCategory::LIFE_QUALITY;
    }
    else if (category == "1")
    {
        return FacilityCategory::ECONOMY;
    }
    else if (category == "2")
    {
        return FacilityCategory::ENVIRONMENT;
    }
    else
    {
        throw std::invalid_argument("Invalid facility category");
    }
}

SelectionPolicy *Auxiliary::createSelectionPolicy(const std::string &policy)
{
    if (policy == "nve")
    {
        return new NaiveSelection();
    }
    else if (policy == "bal")
    {
        return new BalancedSelection(0, 0, 0);
    }
    else if (policy == "eco")
    {
        return new EconomySelection();
    }
    else if (policy == "env")
    {
        return new SustainabilitySelection();
    }
    throw std::runtime_error("non existant policy");
}
