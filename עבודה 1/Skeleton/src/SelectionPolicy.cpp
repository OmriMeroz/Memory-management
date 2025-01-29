#include "SelectionPolicy.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Naive Selection implement
NaiveSelection::NaiveSelection() : SelectionPolicy(), lastSelectedIndex(-1)
{
}

NaiveSelection::NaiveSelection(const int index) : SelectionPolicy(), lastSelectedIndex(index)
{
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size(); // modulo
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "Naive";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(lastSelectedIndex);
}

// end section

// Sustainability Selection implement
SustainabilitySelection::SustainabilitySelection() : SelectionPolicy(), lastSelectedIndex(-1)
{
}

SustainabilitySelection::SustainabilitySelection(const int index) : SelectionPolicy(), lastSelectedIndex(index)
{
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{

    bool found = false;
    for (int i = lastSelectedIndex + 1; i != lastSelectedIndex && !found; i++)
    {
        if (i == static_cast<int>(facilitiesOptions.size()))
        {
            i = 0;
        }

        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            found = true;
            lastSelectedIndex = i;
        }
    }

    return facilitiesOptions[lastSelectedIndex];
}
const string SustainabilitySelection::toString() const
{
    return "Sustainability";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(lastSelectedIndex);
}

// end section

// economySelection
EconomySelection::EconomySelection() : lastSelectedIndex(-1)
{
}

EconomySelection::EconomySelection(const int index) : lastSelectedIndex(index)
{
}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex++;
    int i = lastSelectedIndex;
    while (facilitiesOptions[i % facilitiesOptions.size()].getCategory() != FacilityCategory::ECONOMY)
    {
        i++;
    }
    lastSelectedIndex = i % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const
{
    return "Economy";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(lastSelectedIndex);
}

// end section

// Balanced Selection Class

// constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore)
{
}

// select facility
const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    const FacilityType *selectedFacility;
    int m = std::numeric_limits<int>::max(); // max_value

    for (const FacilityType &ft : facilitiesOptions)
    {
        // calculate values
        int tempLifeQualityScore = LifeQualityScore + ft.getLifeQualityScore();
        int tempEconomyScore = EconomyScore + ft.getEconomyScore();
        int tempEnviromentScore = EnvironmentScore + ft.getEnvironmentScore();

        int d = std::max({tempLifeQualityScore, tempEconomyScore, tempEnviromentScore}) - std::min({tempLifeQualityScore, tempEconomyScore, tempEnviromentScore});

        if (d < m)
        {
            selectedFacility = &ft;
            m = d;
        }
    }

    LifeQualityScore += (selectedFacility->getLifeQualityScore());
    EconomyScore += (selectedFacility->getEconomyScore());
    EnvironmentScore += (selectedFacility->getEnvironmentScore());

    return *selectedFacility;
}

// to string
const string BalancedSelection::toString() const
{
    return "Balanced";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
}

// set - adding the scores that we recived to the fields
void BalancedSelection::setFields(int newLifeQualityScore, int newEconomyScore, int newEnvironmentScore)
{
    LifeQualityScore = newLifeQualityScore;
    EconomyScore = newEconomyScore;
    EnvironmentScore = newEnvironmentScore;
}

// end section