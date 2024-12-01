#pragma once
#include <string>
#include <vector>
#include "Settlement.h"
using namespace std;

/**
 * @class Settlement
 * @brief Represents a settlement with a name and type.
 */

/**
 * @brief Constructs a Settlement object.
 * @param name The name of the settlement.
 * @param type The type of the settlement.
 */
Settlement::Settlement(const string &name, SettlementType type)
: name(name), type(type){};

/**
 * @brief Gets the name of the settlement.
 * @return The name of the settlement.
 */
const string &Settlement::getName() const{
    return name;
};

/**
 * @brief Gets the type of the settlement.
 * @return The type of the settlement.
 */
SettlementType Settlement::getType() const{
    return type;
};

/**
 * @brief Converts the settlement information to a string.
 * @return A string representation of the settlement.
 */
const string Settlement::toString() const{
    switch (type) {
        case SettlementType::VILLAGE: return "SettlementName: " + name + "\nSettlementType: Village";
        case SettlementType::CITY: return "SettlementName: " + name + "\nSettlementType: City";
        case SettlementType::METROPOLIS: return "SettlementName: " + name + "\nSettlementType: Metropolis";
}
