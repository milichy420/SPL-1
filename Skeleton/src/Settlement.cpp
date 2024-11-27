/**
 * @file Settlement.h
 * @brief Defines the Settlement class and related enumerations.
 */

#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

/**
 * @enum SettlementType
 * @brief Enumeration representing different types of settlements.
 */
enum class SettlementType
{
    VILLAGE,    /**< Represents a village settlement type. */
    CITY,       /**< Represents a city settlement type. */
    METROPOLIS, /**< Represents a metropolis settlement type. */
};

/**
 * @class Settlement
 * @brief Represents a settlement with a name and type.
 */
class Settlement
{
public:
    /**
     * @brief Constructs a Settlement object.
     * @param name The name of the settlement.
     * @param type The type of the settlement.
     */
    Settlement(const string &name, SettlementType type)
    : name(name), type(type){};

    /**
     * @brief Gets the name of the settlement.
     * @return The name of the settlement.
     */
    const string &getName() const{
        return name;
    };

    /**
     * @brief Gets the type of the settlement.
     * @return The type of the settlement.
     */
    SettlementType getType() const{
        return type;
    };

    /**
     * @brief Converts the settlement information to a string.
     * @return A string representation of the settlement.
     */
    const string toString() const{
        switch (type) {
            case SettlementType::VILLAGE: return "SettlementName: " + name + "\nSettlementType: Village";
            case SettlementType::CITY: return "SettlementName: " + name + "\nSettlementType: City";
            case SettlementType::METROPOLIS: return "SettlementName: " + name + "\nSettlementType: Metropolis";
    }
    };

private:
    const string name;   /**< The name of the settlement. */
    SettlementType type; /**< The type of the settlement. */
};