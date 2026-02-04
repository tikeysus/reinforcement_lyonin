#include "UCB_Agent.h"
#include <cmath>

UCB_Agent::UCB_Agent(int k, double c) {
    this->k = k;
    this->c = c;
    this->Q.resize(k, 0.0);
    this->N.resize(k, 0);
}

int UCB_Agent::selectAction(int t) {
    int never_exp = this->findNeverExplored();
    if (never_exp != -1) {
        return never_exp;
    }
    return this->findHighestScore(t);
}

void UCB_Agent::update(int action, int reward) {
    this->N[action]++;
    this->Q[action] += 1.0 / this->N[action] * (reward - this->Q[action]);
}

int UCB_Agent::findNeverExplored() {
    for (int i = 0; i < this->k; i++) {
        if (this->N[i] == 0) {
            return i;
        }
    }
    return -1;
}

int UCB_Agent::findHighestScore(int t) {
    double maxi = -1e100;
    int maxi_index = 0;

    for (int i = 0; i < this->k; i++) {
        double score = this->Q[i] + this->c * std::sqrt(std::log((double)t) / this->N[i]);
        if (score > maxi) {
            maxi = score;
            maxi_index = i;
        }
    }
    return maxi_index;
}
