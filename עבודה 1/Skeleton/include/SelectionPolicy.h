#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy
{
public:
    virtual const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) = 0;
    virtual const string toString() const = 0;
    virtual SelectionPolicy *clone() const = 0;
    virtual ~SelectionPolicy() = default;
};

class NaiveSelection : public SelectionPolicy
{
public:
    NaiveSelection();
    NaiveSelection(const int index);
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    NaiveSelection *clone() const override;
    ~NaiveSelection() override = default;

private:
    int lastSelectedIndex;
};

class BalancedSelection : public SelectionPolicy
{
public:
    BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    BalancedSelection *clone() const override;
    ~BalancedSelection() override = default;
    void setFields(int LifeQualityScore, int EconomyScore, int EnvironmentScore);

private:
    int LifeQualityScore;
    int EconomyScore;
    int EnvironmentScore;
};

class EconomySelection : public SelectionPolicy
{
public:
    EconomySelection();
    EconomySelection(const int index);
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    EconomySelection *clone() const override;
    ~EconomySelection() override = default;

private:
    int lastSelectedIndex;
};

class SustainabilitySelection : public SelectionPolicy
{
public:
    SustainabilitySelection();
    SustainabilitySelection(const int index);
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    SustainabilitySelection *clone() const override;
    ~SustainabilitySelection() override = default;

private:
    int lastSelectedIndex;
};