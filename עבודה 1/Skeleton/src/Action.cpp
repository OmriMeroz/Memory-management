#include "Action.h"
#include "Plan.h"
#include "Simulation.h"
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <sstream>
#include <iostream>
#include <algorithm>

extern Simulation *backup;

using namespace std;

std::string typeToInt(SettlementType type)
{
    switch (type)
    {
    case SettlementType::VILLAGE:
        return "0";
    case SettlementType::CITY:
        return "1";
    case SettlementType::METROPOLIS:
        return "2";
    default:
        return "Unknown";
    }
}

std::string statusToString(ActionStatus status)
{
    switch (status)
    {
    case ActionStatus::COMPLETED:
        return "COMPLETED";
    case ActionStatus::ERROR:
        return "ERROR";
    default:
        return "Unknown";
    }
}

std::string FacilityCategoryToString(FacilityCategory category)
{
    switch (category)
    {
    case FacilityCategory::ECONOMY:
        return "ECONOMY";
    case FacilityCategory::ENVIRONMENT:
        return "ENVIRONMENT";
    case FacilityCategory::LIFE_QUALITY:
        return "LIFE_QUALITY";
    default:
        return "Unknown";
    }
}

// Base Action Class
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED)
{
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

// end class

// Simulate Step class
// constructor
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps)
{
}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }

    complete();
}

// to string
const string SimulateStep::toString() const
{
    return "step " + to_string(numOfSteps) + " " + statusToString(getStatus());
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(numOfSteps);
}

// end class

// Add Settlement Class
// constructor
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType)
{
}

void AddSettlement::act(Simulation &simulation)
{
    Settlement *newStellement = new Settlement(settlementName, settlementType);
    if (simulation.isSettlementExists(settlementName))
    {
        error("Settlement alreadt exists");
        cout << getErrorMsg() << endl;
    }
    else
    {
        simulation.addSettlement(newStellement);
        complete();
    }
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(settlementName, settlementType);
}

const string AddSettlement::toString() const
{
    return "settlement " + settlementName + " " + typeToInt(settlementType) + " " + statusToString(getStatus());
}

// end class

// Add Facility Class
// contructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (!simulation.addFacility(newFacility))
    {
        error("Facility already exists");
        cout << getErrorMsg() << endl;
    }
    else
    {
        complete();
    }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
}

const string AddFacility::toString() const
{
    return "facility " + facilityName + " " + FacilityCategoryToString(facilityCategory) + " " + to_string(price) + " " + to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + statusToString(getStatus());
}

// end class

// ChangePlanPolicy Class
// constructor
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy)
{
}

void ChangePlanPolicy::act(Simulation &simulation)
{

    try
    {
        SelectionPolicy *sp = nullptr;
        if (newPolicy == "nve")
        {
            sp = new NaiveSelection();
        }
        else if (newPolicy == "bal")
        {
            sp = new BalancedSelection(0, 0, 0);
        }
        else if (newPolicy == "eco")
        {
            sp = new EconomySelection();
        }
        else if (newPolicy == "env")
        {
            sp = new SustainabilitySelection();
        }

        string st = simulation.getPlan(planId).getSelectionPolicy()->toString();
        if (sp->toString() == st)
        {
            error("Cannot change selection policy");
            cout << getErrorMsg() << endl;
            delete sp;
        }
        else
        {
            simulation.getPlan(planId).setSelectionPolicy(sp);
            cout << "PlanID: " + to_string(planId) << endl;
            cout << "PreviousPolicy: " + st << endl;
            cout << "newPolicy: " + sp->toString() << endl;
            complete();
        }
    }
    catch (const std::runtime_error &e)
    {
        error("Cannot change selection policy");
        cout << getErrorMsg() << endl;
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(planId, newPolicy);
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy " + to_string(planId) + " " + newPolicy + " " + statusToString(getStatus());
}

// end class

// Add Plan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy)
{
}
void AddPlan::act(Simulation &simulation)
{
    if ((!simulation.isSettlementExists(settlementName)) || ((selectionPolicy != "nve" && selectionPolicy != "bal" && selectionPolicy != "eco" && selectionPolicy != "env")))
    {
        error("Cannot create this plan");
        cout << getErrorMsg() << endl;
    }
    else
    {
        SelectionPolicy *sp = nullptr;
        if (selectionPolicy == "nve")
        {
            sp = new NaiveSelection();
        }
        else if (selectionPolicy == "bal")
        {
            sp = new BalancedSelection(0, 0, 0);
        }
        else if (selectionPolicy == "eco")
        {
            sp = new EconomySelection();
        }
        else if (selectionPolicy == "env")
        {
            sp = new SustainabilitySelection();
        }
        simulation.addPlan(simulation.getSettlement(settlementName), sp);
        complete();
    }
}
const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + statusToString(getStatus());
}
AddPlan *AddPlan::clone() const
{
    return new AddPlan(settlementName, selectionPolicy);
}

// end

// Print Plan Status
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId)
{
}
void PrintPlanStatus::act(Simulation &simulation)
{
    try
    {
        Plan plan = simulation.getPlan(planId);
        cout << plan.toString() << endl;
        complete();
    }
    catch (const std::runtime_error &e)
    {
        error("Plan doesn't exist");
        cout << getErrorMsg() << endl;
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(planId);
}

const string PrintPlanStatus::toString() const
{
    return "planStatus " + to_string(planId) + " " + statusToString(getStatus());
}

// end class

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Simulation &simulation)
{
    const auto &actionsLog = simulation.getActionsLog();

    for (const auto &action : actionsLog)
    {
        cout << action->toString() << endl;
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog();
}

const string PrintActionsLog::toString() const
{
    return "log " + statusToString(getStatus());
}

Close::Close()
{
}

void Close::act(Simulation &simulation)
{
    complete();
    simulation.SetIsRunning(false);
    for (const auto &plan : simulation.getPlans())
    {
        std::cout << "Plan ID: " << plan.getID() << std::endl;
        std::cout << "Settlement Name: " << plan.getSettlement().getName() << std::endl;
        std::cout << "Life Quality Score: " << plan.getlifeQualityScore() << std::endl;
        std::cout << "Economy Score: " << plan.getEconomyScore() << std::endl;
        std::cout << "Environment Score: " << plan.getEnvironmentScore() << std::endl;
    }
}

Close *Close::clone() const
{
    return new Close();
}

const string Close::toString() const
{
    return "close " + statusToString(getStatus());
}

// BackupSimulation Class
BackupSimulation::BackupSimulation()
{
}

void BackupSimulation::act(Simulation &simulation)
{
    if (backup != nullptr)
    {
        delete backup;
    }
    backup = new Simulation(simulation);

    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation();
}

const string BackupSimulation::toString() const
{
    return "backup " + statusToString(getStatus());
}

// back up

// restore:

RestoreSimulation::RestoreSimulation()
{
}

void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << getErrorMsg() << endl;
    }
    else
    {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation();
}

const string RestoreSimulation::toString() const
{
    return "restore " + statusToString(getStatus());
}