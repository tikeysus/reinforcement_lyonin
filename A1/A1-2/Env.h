#ifndef ENV_H
#define ENV_H

#include <vector>
#include <random>

class Env {
public:
    Env(int k, int seed);

    void randomize();
    int pull(int action);
    int optimalArm() const;

    int getK() const { return this->k; }
    double getQi(int i) const { return this->q[i]; }

private:
    int k;
    std::vector<double> q;

    std::mt19937 rng;
    std::uniform_real_distribution<double> uni;
};

#endif
