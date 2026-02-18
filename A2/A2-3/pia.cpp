#include "pia.h"
#include "Gridworld.h"
#include "PolicyIteration.h"
#include <iostream>

void runPolicyIteration() {
    double p1, p2, rup, rdown, rright, rleft;
    std::cout << "\n[Policy Iteration]\nEnter p1 p2 rup rdown rright rleft:\n";
    std::cin >> p1 >> p2 >> rup >> rdown >> rright >> rleft;

    if (p1 < 0 || p2 < 0 || p1 + p2 > 1.0) {
        std::cout << "Invalid probabilities: require p1>=0, p2>=0, p1+p2<=1.\n";
        return;
    }

    const int N = 4;
    constexpr double gamma = 0.95;
    constexpr double theta = 0.001;

    const Gridworld mdp(N, p1, p2, rup, rdown, rright, rleft, gamma, theta);
    PolicyIteration pi(mdp);
    pi.run();
}
