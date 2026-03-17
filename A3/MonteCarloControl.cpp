#include "MonteCarloControl.h"

#include <chrono>

using namespace std;

MonteCarloControl::MonteCarloControl(const Gridworld& env, const TrainingOptions& options)
    : env_(env), options_(options) {}

TrainingSummary MonteCarloControl::run() {
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

SingleRunResult MonteCarloControl::runSingle(const uint64_t seed) const {
    mt19937_64 rng(seed);

    SingleRunResult result;
    result.qValues.assign(env_.numStates(), {0.0, 0.0, 0.0, 0.0});

    double normalizedStepsSum = 0.0;
    double normalizedTimeSum = 0.0;

    const auto runStart = chrono::steady_clock::now();

    for (int episode = 0; episode < options_.episodes; ++episode) {
        const auto episodeStart = chrono::steady_clock::now();

        int state = env_.randomNonTerminalState(rng);
        const int distance = env_.manhattanDistanceToGoal(state);
        int action = uniformRandomAction(rng);

        vector<int> states;
        vector<int> actions;
        vector<double> rewards;
        states.reserve(256);
        actions.reserve(256);
        rewards.reserve(256);

        int steps = 0;
        while (steps < options_.maxStepsPerEpisode && !env_.isTerminal(state)) {
            states.push_back(state);
            actions.push_back(action);

            const auto [nextState, reward] = env_.sampleTransition(state, action, rng);
            rewards.push_back(reward);

            ++steps;
            ++result.totalSteps;

            if (env_.isTerminal(nextState)) {
                break;
            }

            action = epsilonGreedyAction(result.qValues[nextState], epsilonForEpisode(options_, episode), rng);
            state = nextState;
        }

        vector<double> returns(rewards.size(), 0.0);
        double G = 0.0;
        for (int t = static_cast<int>(rewards.size()) - 1; t >= 0; --t) {
            G = rewards[t] + options_.gamma * G;
            returns[t] = G;
        }

        vector<array<bool, 4>> visited(env_.numStates(), {false, false, false, false});
        for (size_t t = 0; t < states.size(); ++t) {
            const int visitState = states[t];
            const int visitAction = actions[t];
            if (visited[visitState][visitAction]) {
                continue;
            }

            visited[visitState][visitAction] = true;
            const double& target = returns[t];
            double& estimate = result.qValues[visitState][visitAction];
            estimate += options_.alpha * (target - estimate);
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
