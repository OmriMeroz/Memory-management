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
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, PlanStatus status, vector<Facility *> newFacilities, vector<Facility *> newUnderConstruction, const vector<FacilityType> &facilityOptions, int life_quality_score, int economy_score, int environment_score);
    const int getID() const;
    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    const PlanStatus getStatus() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    const vector<Facility *> &getunderConstruction() const;
    void addFacility(Facility *facility);
    const string toString() const;
    const Settlement getSettlement() const;
    const SelectionPolicy *getSelectionPolicy() const;
    // Rule of 5
    Plan(const Plan &other);            // copy constructor
    Plan &operator=(const Plan &other); // copy assignment operator
    ~Plan();                            // Destructor
    Plan(Plan &&other);                 // move constructor
    Plan &operator=(Plan &&other);      // move assignment operator
    void clear();
    void copy(const Plan &other);

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
};