#include "Simulation.h"
#include "Plan.h"
#include "Action.h"
#include "Facility.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{ // Initialize other members as needed
    std::ifstream configFile(configFilePath);

    if (!configFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + configFilePath);
    }

    string line;
    while (std::getline(configFile, line))
    {
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        if (parsedArgs[0] == "settlement")
        {
            string settlementName = parsedArgs[1];
            int settlementTypeInt = std::stoi(parsedArgs[2]);                               // Convert string to int
            SettlementType settlementType = static_cast<SettlementType>(settlementTypeInt); // Convert int to enum

            settlements.push_back(new Settlement(settlementName, settlementType));
        }
        else if (parsedArgs[0] == "facility")
        {
            string facilityName = parsedArgs[1];
            int categoryInt = std::stoi(parsedArgs[2]); // Convert string to integer
            FacilityCategory category = static_cast<FacilityCategory>(categoryInt);
            int price = std::stoi(parsedArgs[3]);
            int lifeQualityImpact = std::stoi(parsedArgs[4]);
            int ecoImpact = std::stoi(parsedArgs[5]);
            int envImpact = std::stoi(parsedArgs[6]);

            facilitiesOptions.push_back(FacilityType(facilityName, category, price, lifeQualityImpact, ecoImpact, envImpact));
        }
        else if (parsedArgs[0] == "plan")
        {

            SelectionPolicy *policy = nullptr;

            // Check the string and assign the appropriate policy
            if (parsedArgs[2] == "bal")
            {
                policy = new BalancedSelection(0, 0, 0); // Derived from SelectionPolicy
            }
            else if (parsedArgs[2] == "eco")
            {
                policy = new EconomySelection(); // Derived from SelectionPolicy
            }
            else if (parsedArgs[2] == "env")
            {
                policy = new SustainabilitySelection(); // Derived from SelectionPolicy
            }
            else
            {
                policy = new NaiveSelection(); // Derived from SelectionPolicy
            }
            Settlement *targetSettlement = nullptr;
            for (auto &s : settlements)
            {
                if (s->getName() == parsedArgs[1])
                { // Assuming settlementName is known
                    targetSettlement = s;
                    break;
                }
            }
            plans.push_back(Plan(planCounter, *targetSettlement, policy, facilitiesOptions));
            planCounter++;
        }
    }
    configFile.close();
}

void Simulation::start()
{
    open();
    cout << "The simulation has started" << endl;
    while (isRunning)
    {
        BaseAction *action;
        string command;
        getline(cin, command);
        vector<string> arguments = Auxiliary::parseArguments(command);
        const string &requestedAction = arguments[0];
        // checking commands
        if (requestedAction == "plan")
        {
            const string &settlementName = arguments[1];
            const string &selectionPolicy = arguments[2];
            action = new AddPlan(settlementName, selectionPolicy);
        }
        else if (requestedAction == "step")
        {
            action = new SimulateStep(std::stoi(arguments[1]));
        }
        else if (requestedAction == "settlement")
        {
            const string &settlementName = arguments[1];
            switch (std::stoi(arguments[2]))
            {
            case 0:
                action = new AddSettlement(settlementName, SettlementType::VILLAGE);
                break;
            case 1:
                action = new AddSettlement(settlementName, SettlementType::CITY);
                break;
            case 2:
                action = new AddSettlement(settlementName, SettlementType::METROPOLIS);
                break;
            default:
                throw std::runtime_error("Settlement not found");
            }
        }
        else if (requestedAction == "facility")
        {
            string facilityName = arguments[1];
            int categoryInt = std::stoi(arguments[2]); // Convert string to integer
            FacilityCategory category = static_cast<FacilityCategory>(categoryInt);
            int price = std::stoi(arguments[3]);
            int lifeQualityScore = std::stoi(arguments[4]);
            int economyScore = std::stoi(arguments[5]);
            int environmentScore = std::stoi(arguments[6]);
            action = new AddFacility(facilityName, category, price, lifeQualityScore, economyScore, environmentScore);
        }
        else if (requestedAction == "planStatus")
        {
            action = new PrintPlanStatus(std::stoi(arguments[1]));
        }
        else if (requestedAction == "changePolicy")
        {
            ChangePlanPolicy *change = new ChangePlanPolicy(std::stoi(arguments[1]), arguments[2]);
            action = change;
        }
        else if (requestedAction == "log")
        {
            action = new PrintActionsLog();
        }
        else if (requestedAction == "close")
        {
            action = new Close();
        }
        else if (requestedAction == "backup")
        {
            action = new BackupSimulation();
        }
        else if (requestedAction == "restore")
        {
            action = new RestoreSimulation();
        }
        else
        {
            cout << "Command not found" << endl;
            continue;
        }

        action->act(*this);
        actionsLog.push_back(action);
    }
}

void Simulation::step()
{
    for (int i = 0; i < planCounter; i++)
    {
        plans[i].step();
    }
}

void Simulation::close()
{
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}

vector<BaseAction *> Simulation::getActionsLog()
{
    return actionsLog;
}

void Simulation::SetIsRunning(bool isRun)
{
    isRunning = isRun;
}

vector<Plan> Simulation::getPlans()
{
    return plans;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    int planID = planCounter;
    planCounter++;
    Plan p = Plan(planID, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(p);
}
void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}
bool Simulation::addSettlement(Settlement *settlement)
{
    if (isSettlementExists(settlement->getName()))
    {
        return false;
    }
    else
    {
        settlements.push_back(settlement);
        return true;
    }
}

bool Simulation::addFacility(FacilityType facility)
{
    for (const FacilityType &f : facilitiesOptions)
    {
        if (f.getName() == facility.getName())
        {
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (const auto &settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}
Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (auto &settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return *settlement;
        }
    }
    throw std::runtime_error("Settlement not found");
}
Plan &Simulation::getPlan(const int planID)
{
    for (auto &plan : plans)
    {
        if (plan.getID() == planID)
        {
            return plan;
        }
    }
    throw std::runtime_error("Plan not found");
}

// //         // ____________Rule of 5 __________________
// //         // ____________Rule of 5 __________________
// //         // ____________Rule of 5 __________________

void Simulation::clear()
{

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    for (Settlement *settel : settlements)
    {
        delete settel;
    }
    settlements.clear();

    plans.clear();
    facilitiesOptions.clear();
}

void Simulation::copy(const Simulation &other)
{

    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone()); // Cloning each action polymorphically
    }

    for (Settlement *settel : other.settlements)
    {
        settlements.push_back(new Settlement(settel->getName(), settel->getType()));
    }
    for (Plan p : other.plans)
    {
        this->plans.push_back(Plan(p));
    }
    for (FacilityType f : other.facilitiesOptions)
    {
        this->facilitiesOptions.push_back(FacilityType(f));
    }
}

Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning),
                                                  planCounter(other.planCounter), // For assigning unique plan IDs
                                                  actionsLog(),
                                                  plans(),
                                                  settlements(),
                                                  facilitiesOptions()
{
    for (Settlement *settel : settlements)
    {
        delete settel;
    }

    for (Settlement *settel : other.settlements)
    {
        settlements.push_back(new Settlement(*settel));
    }

    for (Plan p : other.plans)
    {
        Plan pnew(p.getID(), this->getSettlement(p.getSettlement().getName()), p.getSelectionPolicy()->clone(), p.getStatus(), p.getFacilities(), p.getunderConstruction(), facilitiesOptions, p.getlifeQualityScore(), p.getEconomyScore(), p.getEnvironmentScore());
        plans.push_back(pnew);
    }

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }

    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone()); // Cloning each action polymorphically
    }

    for (FacilityType f : other.facilitiesOptions)
    {
        this->facilitiesOptions.push_back(FacilityType(f));
    }
}

Simulation &Simulation::operator=(const Simulation &other)
{
    if (this != &other)
    {
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (Settlement *settel : settlements)
        {
            delete settel;
        }
        settlements.clear();

        for (Settlement *settel : other.settlements)
        {
            settlements.push_back(new Settlement(*settel));
        }
        plans.clear();

        for (Plan p : other.plans)
        {
            Plan pnew(p.getID(), this->getSettlement(p.getSettlement().getName()), p.getSelectionPolicy()->clone(), p.getStatus(), p.getFacilities(), p.getunderConstruction(), facilitiesOptions, p.getlifeQualityScore(), p.getEconomyScore(), p.getEnvironmentScore());
            plans.push_back(pnew);
        }

        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();

        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone()); // Cloning each action polymorphically
        }

        facilitiesOptions.clear();
        for (FacilityType f : other.facilitiesOptions)
        {
            this->facilitiesOptions.push_back(FacilityType(f));
        }
    }
    return *this;
}

Simulation::~Simulation()
{
    clear();
}

Simulation::Simulation(Simulation &&other) : isRunning(other.isRunning),
                                             planCounter(other.planCounter),
                                             actionsLog(other.actionsLog),
                                             plans(other.plans),
                                             settlements(other.settlements),
                                             facilitiesOptions(other.facilitiesOptions)
{
    other.actionsLog.clear();
    other.settlements.clear();
}

Simulation &Simulation::operator=(Simulation &&other)
{
    if (this != &other)
    {
        // Clean up existing resources
        clear();

        // copy fields
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans = other.plans;
        actionsLog = other.actionsLog;
        settlements = other.settlements;

        other.actionsLog.clear();
        other.settlements.clear();
    }
    return *this;
}