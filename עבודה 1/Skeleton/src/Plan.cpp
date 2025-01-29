#include "Plan.h"
#include "SelectionPolicy.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

// constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0)
{
}

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, PlanStatus status, vector<Facility *> newFacilities, vector<Facility *> newUnderConstruction, const vector<FacilityType> &facilityOptions, int life_quality_score, int economy_score, int environment_score) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(status), facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(life_quality_score), economy_score(economy_score), environment_score(environment_score)
{
    for (Facility *facility : newFacilities)
    {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility *facility : newUnderConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

const int Plan::getID() const
{
    return plan_id;
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const PlanStatus Plan::getStatus() const
{
    return status;
}

void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy)
{
    if (typeid(*newSelectionPolicy) == typeid(BalancedSelection))
    {
        int life = life_quality_score;
        int eco = economy_score;
        int env = environment_score;

        for (int i = 0; i < (int)underConstruction.size(); i++)
        {
            life += underConstruction[i]->getLifeQualityScore();
            eco += underConstruction[i]->getEconomyScore();
            env += underConstruction[i]->getEnvironmentScore();
        }

        (static_cast<BalancedSelection *>(newSelectionPolicy))->setFields(life, eco, env);
    }
    delete selectionPolicy;
    this->selectionPolicy = newSelectionPolicy;
}

void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        int facilitiesToBuild = settlement.facilitiesNum() - underConstruction.size();
        for (int i = 1; i <= facilitiesToBuild; i++)
        {
            Facility *currFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            underConstruction.push_back(currFacility);
        }
    }
    for (int i = 0; i < (int)underConstruction.size(); i++)
    {
        (underConstruction[i])->step();
        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEconomyScore();
            environment_score += underConstruction[i]->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
            i--;
        }
    }

    if ((int)underConstruction.size() >= settlement.facilitiesNum())
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}

std::string statusToString(PlanStatus status)
{
    switch (status)
    {
    case PlanStatus::AVALIABLE:
        return "AVALIABLE";
    case PlanStatus::BUSY:
        return "BUSY";
    default:
        return "Unknown";
    }
}

void Plan::printStatus()
{
    cout << statusToString(status) << endl;
}

const string Plan::toString() const
{
    std::ostringstream oss;
    oss << "PlanID: " << this->getID() << "\n";
    oss << "SettlementName: " << this->settlement.getName() << "\n";
    oss << "PlanStatus: " << statusToString(this->status) << "\n";
    string sp = "";
    if (typeid(*this->selectionPolicy) == typeid(NaiveSelection))
    {
        sp = "nve";
    }
    else if (typeid(*this->selectionPolicy) == typeid(BalancedSelection))
    {
        sp = "bal";
    }
    else if (typeid(*this->selectionPolicy) == typeid(EconomySelection))
    {
        sp = "eco";
    }
    else if (typeid(*this->selectionPolicy) == typeid(SustainabilitySelection))
    {
        sp = "env";
    }
    oss << "SelectionPolicy: " << sp << "\n";
    oss << "LifeQualityScore: " << this->getlifeQualityScore() << "\n";
    oss << "EconomyScore: " << this->getEconomyScore() << "\n";
    oss << "EnvironmentScore: " << this->getEnvironmentScore() << "\n";

    for (Facility *fac : this->facilities)
    {
        oss << "FacilityName: " << fac->getName() << "\n";
        oss << "FacilityStatus: OPERATIONAL" << "\n";
    }

    for (Facility *uc : this->underConstruction)
    {
        oss << "FacilityName: " << uc->getName() << "\n";
        oss << "FacilityStatus: UNDER_CONSTRUCTIONS" << "\n";
    }

    return oss.str();
}

const Settlement Plan::getSettlement() const
{
    return settlement;
}

const SelectionPolicy *Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}
const vector<Facility *> &Plan::getunderConstruction() const
{
    return underConstruction;
}
void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
}
// Rule of 5
///////////////////////////////////////

void Plan::clear()
{
    for (Facility *facility : facilities)
    {
        delete facility;
    }
    facilities.clear();

    for (Facility *facility : underConstruction)
    {
        delete facility;
    }
    underConstruction.clear();

    if (selectionPolicy)
    {
        delete selectionPolicy;
        selectionPolicy = nullptr;
    }
}

void Plan::copy(const Plan &other)
{
    selectionPolicy = other.selectionPolicy->clone();

    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(nullptr),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    copy(other);
}

Plan &Plan::operator=(const Plan &other)
{
    if (this != &other)
    {
        // Clean up existing resources
        clear();

        // copy fields
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        copy(other);

        // settlement and facilityOptions are const references, no reassignment needed
    }
    return *this;
}

Plan::~Plan()
{
    clear();
}

Plan::Plan(Plan &&other) : plan_id(other.plan_id),
                           settlement(other.settlement),
                           selectionPolicy(other.selectionPolicy),
                           status(other.status),
                           facilities(other.facilities),
                           underConstruction(other.underConstruction),
                           facilityOptions(other.facilityOptions),
                           life_quality_score(other.life_quality_score),
                           economy_score(other.economy_score),
                           environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

Plan &Plan::operator=(Plan &&other)
{
    if (this != &other)
    {
        // Clean up existing resources
        clear();

        // copy fields
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        facilities = other.facilities;
        underConstruction = other.underConstruction;
        selectionPolicy = other.selectionPolicy;

        other.selectionPolicy = nullptr;
        other.facilities.clear();
        other.underConstruction.clear();
    }
    return *this;
}