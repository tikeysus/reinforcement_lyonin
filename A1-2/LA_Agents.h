#ifndef LA_AGENTS_H
#define LA_AGENTS_H

#include <vector>
#include <random>

class LearningAutomatonBase {
public:
    LearningAutomatonBase(int k, double alpha, double beta, int seed);

    int selectAction();
    virtual void update(int action, int reward) = 0;

    // optional debug:
    double getP(int i) const { return this->p[i]; }

protected:
    int k;
    double alpha;
    double beta;
    std::vector<double> p;

    std::mt19937 rng;
    std::uniform_real_distribution<double> uni;

    int sampleFromP();
};

class LRP_Agent : public LearningAutomatonBase {
public:
    LRP_Agent(int k, double alpha, double beta, int seed);
    void update(int action, int reward) override;
};

class LRI_Agent : public LearningAutomatonBase {
public:
    LRI_Agent(int k, double alpha, int seed); // beta=0
    void update(int action, int reward) override;
};

#endif
