#ifndef A3_3_RLCOMMON_H
#define A3_3_RLCOMMON_H

#include "Gridworld.h"

#include <array>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

enum class EpsilonMode {
    Fixed,
    LinearDecay
};

struct TrainingOptions {
    double p1 = -1.0;
    double p2 = -1.0;
    double alpha = 0.1;
    double gamma = 0.9;
    double epsilon = 0.1;
    int episodes = 25000;
    int maxStepsPerEpisode = 10000;
    int runs = 1;
    std::uint64_t seed = 20260317ULL;
    EpsilonMode epsilonMode = EpsilonMode::Fixed;
    bool csv = false;
    bool quiet = false;
};

struct SingleRunResult {
    std::vector<std::array<double, 4>> qValues;
    long long totalSteps = 0;
    double totalTimeMs = 0.0;
    double meanNormalizedSteps = 0.0;
    double meanNormalizedTimeMs = 0.0;
};

struct TrainingSummary {
    std::vector<std::array<double, 4>> averageQValues;
    int episodesPerRun = 0;
    int runs = 0;
    long long totalSteps = 0;
    double totalTimeMs = 0.0;
    double meanNormalizedSteps = 0.0;
    double meanNormalizedTimeMs = 0.0;
};

bool parseCommonArguments(int argc, char** argv, TrainingOptions& options);
bool promptForProbabilities(TrainingOptions& options);
double epsilonForEpisode(const TrainingOptions& options, int episodeIndex);
int uniformRandomAction(std::mt19937_64& rng);
int epsilonGreedyAction(const std::array<double, 4>& qValues, double epsilon, std::mt19937_64& rng);
std::vector<int> greedyActions(const std::array<double, 4>& qValues, double tolerance = 1e-9);
int randomGreedyAction(const std::array<double, 4>& qValues, std::mt19937_64& rng, double tolerance = 1e-9);
double expectedEpsilonGreedyValue(const std::array<double, 4>& qValues, double epsilon);
std::string epsilonModeName(EpsilonMode mode);
void printSummary(const std::string& algorithmName,
                  const TrainingOptions& options,
                  const TrainingSummary& summary,
                  const Gridworld& env);
void printUsage(const std::string& executableName, const std::string& algorithmName);

#endif
