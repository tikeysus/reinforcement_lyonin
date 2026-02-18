#include "LA_Agents.h"

LearningAutomatonBase::LearningAutomatonBase(int k, double alpha, double beta, int seed)
    : k(k), alpha(alpha), beta(beta), p(k), rng(seed), uni(0.0, 1.0) {
    // uniform init
    for (int i = 0; i < this->k; i++) {
        this->p[i] = 1.0 / this->k;
    }
}

int LearningAutomatonBase::selectAction() {
    return this->sampleFromP();
}

int LearningAutomatonBase::sampleFromP() {
    double u = this->uni(this->rng);
    double cumulative = 0.0;

    for (int i = 0; i < this->k; i++) {
        cumulative += this->p[i];
        if (u < cumulative) {
            return i;
        }
    }
    // floating point safety
    return this->k - 1;
}

LRP_Agent::LRP_Agent(int k, double alpha, double beta, int seed)
    : LearningAutomatonBase(k, alpha, beta, seed) {}

void LRP_Agent::update(int action, int reward) {
    // reward = 1: linear reward
    if (reward == 1) {
        // chosen action increases
        this->p[action] = this->p[action] + this->alpha * (1.0 - this->p[action]);
        // others decrease proportionally
        for (int j = 0; j < this->k; j++) {
            if (j == action) continue;
            this->p[j] = (1.0 - this->alpha) * this->p[j];
        }
        return;
    }

    // reward = 0: linear penalty
    // chosen action decreases
    this->p[action] = (1.0 - this->beta) * this->p[action];

    // others get a share of beta/(k-1) plus scaled old prob
    double add = this->beta / (this->k - 1);
    for (int j = 0; j < this->k; j++) {
        if (j == action) continue;
        this->p[j] = add + (1.0 - this->beta) * this->p[j];
    }

    // normalization so the probabilities sum to 1
    double sum = 0.0;
    for (int i = 0; i < this->k; i++) sum += this->p[i];
    for (int i = 0; i < this->k; i++) this->p[i] /= sum;
}

LRI_Agent::LRI_Agent(int k, double alpha, int seed)
    : LearningAutomatonBase(k, alpha, 0.0, seed) {}

void LRI_Agent::update(int action, int reward) {
    // only update on reward
    if (reward == 0) {
        return;
    }

    // same as reward case in LRP
    this->p[action] = this->p[action] + this->alpha * (1.0 - this->p[action]);
    for (int j = 0; j < this->k; j++) {
        if (j == action) continue;
        this->p[j] = (1.0 - this->alpha) * this->p[j];
    }

    // normalization so the probabilities sum to 1
    double sum = 0.0;
    for (int i = 0; i < this->k; i++) sum += this->p[i];
    for (int i = 0; i < this->k; i++) this->p[i] /= sum;
}
