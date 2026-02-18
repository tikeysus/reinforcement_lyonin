#include "PolicyIteration.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

static array<double,4> greedyProbsFromQ(const array<double,4>& q) {
    double mx = *ranges::max_element(q);
    int cnt = 0;
    for (double v : q) if (fabs(v - mx) < 1e-12) cnt++;

    array<double,4> probs{};
    for (int a = 0; a < 4; a++) {
        probs[a] = (fabs(q[a] - mx) < 1e-12) ? (1.0 / cnt) : 0.0;
    }
    return probs;
}

PolicyIteration::PolicyIteration(const Gridworld& mdp)
    : mdp(mdp),
      V(mdp.numStates(), 0.0),
      pi(mdp.numStates())
{
    // equiprobable random policy
    for (int s = 0; s < mdp.numStates(); s++) {
        pi[s] = {0.25, 0.25, 0.25, 0.25};
    }
}

vector<double> PolicyIteration::policyEvaluation() {
    const int S = mdp.numStates();
    const int A = mdp.numActions();
    const double gamma = mdp.getGamma();
    const double theta = mdp.getTheta();

    while (true) {
        double delta = 0.0;

        for (int s = 0; s < S; s++) {
            if (mdp.isTerminal(s)) continue;

            double vold = V[s];
            double vnew = 0.0;

            for (int a = 0; a < A; a++) {
                double pa = pi[s][a];
                if (pa == 0.0) continue;

                double q = 0.0;
                for (const auto& tr : mdp.transitions(s, a)) {
                    q += tr.prob * (tr.reward + gamma * V[tr.s2]);
                }
                vnew += pa * q;
            }

            V[s] = vnew;
            delta = max(delta, fabs(vnew - vold));
        }

        if (delta < theta) break;
    }
    return V;
}

bool PolicyIteration::policyImprovement() {
    const int S = mdp.numStates();
    const int A = mdp.numActions();
    const double gamma = mdp.getGamma();

    bool stable = true;

    for (int s = 0; s < S; s++) {
        if (mdp.isTerminal(s)) continue;

        array<double,4> Q{};
        for (int a = 0; a < A; a++) {
            double q = 0.0;
            for (const auto& tr : mdp.transitions(s, a)) {
                q += tr.prob * (tr.reward + gamma * V[tr.s2]);
            }
            Q[a] = q;
        }

        array<double,4> newPi = greedyProbsFromQ(Q);
        if (newPi != pi[s]) stable = false;
        pi[s] = newPi;
    }

    return stable;
}

void PolicyIteration::run() {
    int outer = 0;

    while (true) {
        outer++;

        auto t0= chrono::high_resolution_clock::now();

        // evaluate current policy
        policyEvaluation();

        // improve policy
        bool stable = policyImprovement();

        auto t1 = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();

        cout << "PI outer iter " << outer
             << " time(ms)=" << fixed << setprecision(3) << ms << "\n";

        if (stable) {
            cout << "\nTotal PI outer iterations: " << outer << "\n";
            mdp.printPolicyGreedyFromV(V);
            break;
        }
    }
}
