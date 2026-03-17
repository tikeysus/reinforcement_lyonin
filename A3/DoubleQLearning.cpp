#include "DoubleQLearning.h"

#include <array>
#include <chrono>
#include <vector>

using namespace std;

DoubleQLearning::DoubleQLearning(const Gridworld& env, const TrainingOptions& options)
    : env_(env), options_(options) {}

TrainingSummary DoubleQLearning::run() {
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

SingleRunResult DoubleQLearning::runSingle(const uint64_t seed) const {
    mt19937_64 rng(seed);
    uniform_real_distribution<double> coin(0.0, 1.0);

    SingleRunResult result;
    result.qValues.assign(env_.numStates(), {0.0, 0.0, 0.0, 0.0});

    vector<array<double, 4>> q1(env_.numStates(), {0.0, 0.0, 0.0, 0.0});
    vector<array<double, 4>> q2(env_.numStates(), {0.0, 0.0, 0.0, 0.0});

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
            array<double, 4> combined{};
            for (int action = 0; action < Gridworld::numActions(); ++action) {
                combined[action] = q1[state][action] + q2[state][action];
            }

            const int action = epsilonGreedyAction(combined, epsilon, rng);
            const auto [nextState, reward] = env_.sampleTransition(state, action, rng);

            ++steps;
            ++result.totalSteps;

            if (coin(rng) < 0.5) {
                double tdTarget = reward;
                if (!env_.isTerminal(nextState)) {
                    const int bestAction = randomGreedyAction(q1[nextState], rng);
                    tdTarget += options_.gamma * q2[nextState][bestAction];
                }
                q1[state][action] += options_.alpha * (tdTarget - q1[state][action]);
            } else {
                double tdTarget = reward;
                if (!env_.isTerminal(nextState)) {
                    const int bestAction = randomGreedyAction(q2[nextState], rng);
                    tdTarget += options_.gamma * q1[nextState][bestAction];
                }
                q2[state][action] += options_.alpha * (tdTarget - q2[state][action]);
            }

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

    for (int state = 0; state < env_.numStates(); ++state) {
        for (int action = 0; action < Gridworld::numActions(); ++action) {
            result.qValues[state][action] = q1[state][action] + q2[state][action];
        }
    }

    const auto runEnd = chrono::steady_clock::now();
    result.totalTimeMs = chrono::duration<double, milli>(runEnd - runStart).count();
    result.meanNormalizedSteps = normalizedStepsSum / static_cast<double>(options_.episodes);
    result.meanNormalizedTimeMs = normalizedTimeSum / static_cast<double>(options_.episodes);
    return result;
}
