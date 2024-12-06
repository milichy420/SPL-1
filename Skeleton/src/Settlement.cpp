#include <string>
#include <vector>
#include "Settlement.h"
using namespace std;

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {};

const string &Settlement::getName() const
{
    return name;
};

SettlementType Settlement::getType() const
{
    return type;
};

const string Settlement::toString() const
{
    switch (type)
    {
    case SettlementType::VILLAGE:
        return "SettlementName: " + name + "\nSettlementType: Village";
    case SettlementType::CITY:
        return "SettlementName: " + name + "\nSettlementType: City";
    case SettlementType::METROPOLIS:
        return "SettlementName: " + name + "\nSettlementType: Metropolis";
    }
    return "SettlementName: " + name + "\nSettlementType: Unknown";
};
