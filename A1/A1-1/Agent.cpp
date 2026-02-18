#include "Agent.h"
#include <cmath>

Agent::Agent(int k, double c) {
    this->Q.resize(k, 0.0);
    this->N.resize(k, 0);
    this->k = k;
    this->c = c;
}

int Agent::selectAction(int t) {
    int never_exp = findNeverExplored();
    if (never_exp != -1) {
        return never_exp;
    }
    return findHighestScore(t);
}

void Agent::update(int action, double reward) {
    this->N[action]++;
    Q[action] += 1.0/this->N[action] * (reward - Q[action]);
}

// -1 means we don't have any
int Agent::findNeverExplored() {
    for (int i = 0; i < this->k; i++) {
        if (this->N[i] == 0) {
            return i;
        }
    }
    return -1;
}

int Agent::findHighestScore(int t) {
    double maxi = this->Q[0] + c * std::sqrt(std::log(t)/N[0]);
    int maxi_index = 0;
    for (int i = 1; i < this->k; i++) {
        double score = this->Q[i] + c * std::sqrt(std::log(t)/N[i]);
        if (score > maxi) {
            maxi = score;
            maxi_index = i;
        }
    }
    return maxi_index;
}