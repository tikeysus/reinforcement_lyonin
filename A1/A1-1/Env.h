#ifndef A1_ENV_H
#define A1_ENV_H

#include <random>
#include <vector>

class Env {
public:
    Env(int k, int seed);

    // randomize q values
    void randomize();
    // pulls arm with probability q[a]
    int pull(int action);
    // best arm with highest q value
    int optimalArm();

    int getK() const { return k; };
    double getQi(int i) const { return q[i]; }

private:
    int k;
    std::vector<double> q;
    std::mt19937 rng;
    std::uniform_real_distribution<double> uni;
};
#endif