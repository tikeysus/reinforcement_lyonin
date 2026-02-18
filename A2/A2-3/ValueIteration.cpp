#include "ValueIteration.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ValueIteration::ValueIteration(const Gridworld& mdp)
    : mdp(mdp), V(mdp.numStates(), 0.0) {}

void ValueIteration::run() {
    const int S = mdp.numStates();
    const int A = mdp.numActions();
    const double gamma = mdp.getGamma();
    const double theta = mdp.getTheta();

    int iters = 0;

    while (true) {
        iters++;

        auto t0 = chrono::high_resolution_clock::now();

        double delta = 0.0;
        vector<double> Vnew = V;

        for (int s = 0; s < S; s++) {
            if (mdp.isTerminal(s)) continue;

            double best = -1e100;
            for (int a = 0; a < A; a++) {
                double q = 0.0;
                for (const auto& tr : mdp.transitions(s, a)) {
                    q += tr.prob * (tr.reward + gamma * V[tr.s2]);
                }
                best = max(best, q);
            }
            Vnew[s] = best;
            delta = max(delta, fabs(Vnew[s] - V[s]));
        }

        V.swap(Vnew);

        auto t1 = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();

        cout << "VI iter " << iters
             << " time(ms)=" << fixed << setprecision(3) << ms
             << " delta=" << delta << "\n";

        if (delta < theta) {
            cout << "\nTotal VI iterations: " << iters << "\n";
            mdp.printPolicyGreedyFromV(V);
            break;
        }
    }
}
