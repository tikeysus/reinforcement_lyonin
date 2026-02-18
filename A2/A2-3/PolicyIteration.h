#ifndef A2_3_POLICYITERATION_H
#define A2_3_POLICYITERATION_H

#include "Gridworld.h"
#include <vector>
#include <array>

class PolicyIteration {
public:
    explicit PolicyIteration(const Gridworld& mdp);

    // runs PI until stable policy
    void run();

private:
    const Gridworld& mdp;
    std::vector<double> V;
    std::vector<std::array<double,4>> pi;

    std::vector<double> policyEvaluation();
    bool policyImprovement();
};

#endif //A2_3_POLICYITERATION_H