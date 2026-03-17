#ifndef A3_3_EXPECTEDSARSA_H
#define A3_3_EXPECTEDSARSA_H

#include "Gridworld.h"
#include "RLCommon.h"

class ExpectedSarsa {
public:
    ExpectedSarsa(const Gridworld& env, const TrainingOptions& options);

    TrainingSummary run();

private:
    const Gridworld& env_;
    TrainingOptions options_;

    SingleRunResult runSingle(std::uint64_t seed) const;
};

#endif
