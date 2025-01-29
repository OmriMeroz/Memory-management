#include "Settlement.h"
#include <iostream>
#include <algorithm>

std::string typeToString(SettlementType type)
{
    switch (type)
    {
    case SettlementType::VILLAGE:
        return "Village";
    case SettlementType::CITY:
        return "City";
    case SettlementType::METROPOLIS:
        return "Mertropolis";
    default:
        return "Unknown";
    }
}
// Constructor
Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type)
{
}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const std::string Settlement::toString() const
{
    return "name:" + name + " type:" + typeToString(type);
}

int Settlement::facilitiesNum() const
{
    if (type == SettlementType::VILLAGE)
    {
        return 1;
    }
    if (type == SettlementType::CITY)
    {
        return 2;
    }
    if (type == SettlementType::METROPOLIS)
    {
        return 3;
    }
    return -1;
}

Settlement Settlement::clone() const
{
    return Settlement(name, type);
}
