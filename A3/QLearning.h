#ifndef A3_3_QLEARNING_H
#define A3_3_QLEARNING_H

#include "Gridworld.h"
#include "RLCommon.h"

class QLearning {
public:
    QLearning(const Gridworld& env, const TrainingOptions& options);

    TrainingSummary run();

private:
    const Gridworld& env_;
    TrainingOptions options_;

    SingleRunResult runSingle(std::uint64_t seed) const;
};

#endif
