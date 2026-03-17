#include "QLearning.h"

#include <algorithm>
#include <chrono>

using namespace std;

QLearning::QLearning(const Gridworld& env, const TrainingOptions& options)
    : env_(env), options_(options) {}

TrainingSummary QLearning::run() {
    TrainingSummary summary;
    summary.averageQValues.assign(env_.numStates(), {0.0, 0.0, 0.0, 0.0});
    summary.episodesPerRun = options_.episodes;
    summary.runs = options_.runs;

    for (int run = 0; run < options_.runs; ++run) {
        const SingleRunResult result = runSingle(options_.seed + 104729ULL * static_cast<uint64_t>(run));
        summary.totalSteps += result.totalSteps;
        summary.totalTimeMs += result.totalTimeMs;
        summary.meanNormalizedSteps += result.meanNormalizedSteps;
        summary.meanNormalizedTimeMs += result.meanNormalizedTimeMs;

        for (int state = 0; state < env_.numStates(); ++state) {
            for (int action = 0; action < Gridworld::numActions(); ++action) {
                summary.averageQValues[state][action] += result.qValues[state][action];
            }
        }
    }

    for (int state = 0; state < env_.numStates(); ++state) {
        for (int action = 0; action < Gridworld::numActions(); ++action) {
            summary.averageQValues[state][action] /= static_cast<double>(options_.runs);
        }
    }

    summary.meanNormalizedSteps /= static_cast<double>(options_.runs);
    summary.meanNormalizedTimeMs /= static_cast<double>(options_.runs);
    return summary;
}

SingleRunResult QLearning::runSingle(const uint64_t seed) const {
    mt19937_64 rng(seed);

    SingleRunResult result;
    result.qValues.assign(env_.numStates(), {0.0, 0.0, 0.0, 0.0});

    double normalizedStepsSum = 0.0;
    double normalizedTimeSum = 0.0;

    const auto runStart = chrono::steady_clock::now();

    for (int episode = 0; episode < options_.episodes; ++episode) {
        const auto episodeStart = chrono::steady_clock::now();

        const double epsilon = epsilonForEpisode(options_, episode);
        int state = env_.randomNonTerminalState(rng);
        const int distance = env_.manhattanDistanceToGoal(state);

        int steps = 0;
        while (steps < options_.maxStepsPerEpisode && !env_.isTerminal(state)) {
            const int action = epsilonGreedyAction(result.qValues[state], epsilon, rng);
            const auto [nextState, reward] = env_.sampleTransition(state, action, rng);

            ++steps;
            ++result.totalSteps;

            const double bestNext = env_.isTerminal(nextState)
                ? 0.0
                : *max_element(result.qValues[nextState].begin(), result.qValues[nextState].end());
            const double tdTarget = reward + options_.gamma * bestNext;
            double& estimate = result.qValues[state][action];
            estimate += options_.alpha * (tdTarget - estimate);

            if (env_.isTerminal(nextState)) {
                break;
            }
            state = nextState;
        }

        const auto episodeEnd = chrono::steady_clock::now();
        const double episodeMs = chrono::duration<double, milli>(episodeEnd - episodeStart).count();
        normalizedStepsSum += static_cast<double>(steps) / static_cast<double>(distance);
        normalizedTimeSum += episodeMs / static_cast<double>(distance);
    }

    const auto runEnd = chrono::steady_clock::now();
    result.totalTimeMs = chrono::duration<double, milli>(runEnd - runStart).count();
    result.meanNormalizedSteps = normalizedStepsSum / static_cast<double>(options_.episodes);
    result.meanNormalizedTimeMs = normalizedTimeSum / static_cast<double>(options_.episodes);
    return result;
}
