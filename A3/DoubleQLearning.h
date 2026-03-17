#ifndef A3_3_DOUBLEQLEARNING_H
#define A3_3_DOUBLEQLEARNING_H

#include "Gridworld.h"
#include "RLCommon.h"

class DoubleQLearning {
public:
    DoubleQLearning(const Gridworld& env, const TrainingOptions& options);

    TrainingSummary run();

private:
    const Gridworld& env_;
    TrainingOptions options_;

    SingleRunResult runSingle(std::uint64_t seed) const;
};

#endif
