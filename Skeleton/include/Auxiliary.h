#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

class Auxiliary
{
public:
    static std::vector<std::string> parseArguments(const std::string &line);
    static SettlementType parseSettlementType(const std::string &type);
    static FacilityCategory parseFacilityCategory(const std::string &category);
    static SelectionPolicy createSelectionPolicy(const std::string &policy, const std::vector<FacilityType> &facilityOptions);
};
