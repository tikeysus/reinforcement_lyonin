#ifndef A2_3_VALUEITERATION_H
#define A2_3_VALUEITERATION_H

#include "Gridworld.h"
#include <vector>

class ValueIteration {
public:
    explicit ValueIteration(const Gridworld& mdp);

    // runs VI until delta < theta
    void run();

private:
    const Gridworld& mdp;
    std::vector<double> V;
};

#endif