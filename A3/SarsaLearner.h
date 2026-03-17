#ifndef A3_3_SARSALEARNER_H
#define A3_3_SARSALEARNER_H

#include "Gridworld.h"
#include "RLCommon.h"

class Sarsa {
public:
    Sarsa(const Gridworld& env, const TrainingOptions& options);

    TrainingSummary run();

private:
    const Gridworld& env_;
    TrainingOptions options_;

    SingleRunResult runSingle(std::uint64_t seed) const;
};

#endif
