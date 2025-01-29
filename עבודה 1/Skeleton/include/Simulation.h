#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation
{
public:
    Simulation(const string &configFilePath);
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    void step();
    void close();
    void open();
    vector<BaseAction *> getActionsLog();
    void SetIsRunning(bool isRun);
    vector<Plan> getPlans();

    // Rule of 5
    Simulation(const Simulation &other);            // copy constructor
    Simulation &operator=(const Simulation &other); // copy assignment operator
    ~Simulation();                                  // Destructor
    Simulation(Simulation &&other);                 // move constructor
    Simulation &operator=(Simulation &&other);      // move assignment operator
    void clear();
    void copy(const Simulation &other);

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};