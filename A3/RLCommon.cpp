#include "RLCommon.h"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
bool parseDoubleValue(const string& text, double& value) {
    try {
        size_t processed = 0;
        value = stod(text, &processed);
        return processed == text.size();
    } catch (const exception&) {
        return false;
    }
}

bool parseIntValue(const string& text, int& value) {
    try {
        size_t processed = 0;
        value = stoi(text, &processed);
        return processed == text.size();
    } catch (const exception&) {
        return false;
    }
}

bool parseUnsignedValue(const string& text, uint64_t& value) {
    try {
        size_t processed = 0;
        value = stoull(text, &processed);
        return processed == text.size();
    } catch (const exception&) {
        return false;
    }
}

bool validateOptions(const TrainingOptions& options) {
    if (options.alpha <= 0.0 || options.alpha > 1.0) {
        cerr << "Invalid alpha: require 0 < alpha <= 1.\n";
        return false;
    }
    if (options.gamma < 0.0 || options.gamma > 1.0) {
        cerr << "Invalid gamma: require 0 <= gamma <= 1.\n";
        return false;
    }
    if (options.epsilon < 0.0 || options.epsilon > 1.0) {
        cerr << "Invalid epsilon: require 0 <= epsilon <= 1.\n";
        return false;
    }
    if (options.episodes <= 0 || options.runs <= 0 || options.maxStepsPerEpisode <= 0) {
        cerr << "Episodes, runs, and max steps must be positive.\n";
        return false;
    }
    return true;
}
}  // namespace

bool parseCommonArguments(const int argc, char** argv, TrainingOptions& options) {
    for (int i = 1; i < argc; ++i) {
        const string arg = argv[i];

        auto requireValue = [&](const string& name) -> const char* {
            if (i + 1 >= argc) {
                cerr << "Missing value for " << name << ".\n";
                return nullptr;
            }
            return argv[++i];
        };

        if (arg == "--p1") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseDoubleValue(value, options.p1)) {
                cerr << "Invalid numeric value for --p1.\n";
                return false;
            }
        } else if (arg == "--p2") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseDoubleValue(value, options.p2)) {
                cerr << "Invalid numeric value for --p2.\n";
                return false;
            }
        } else if (arg == "--alpha") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseDoubleValue(value, options.alpha)) {
                cerr << "Invalid numeric value for --alpha.\n";
                return false;
            }
        } else if (arg == "--gamma") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseDoubleValue(value, options.gamma)) {
                cerr << "Invalid numeric value for --gamma.\n";
                return false;
            }
        } else if (arg == "--epsilon") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseDoubleValue(value, options.epsilon)) {
                cerr << "Invalid numeric value for --epsilon.\n";
                return false;
            }
        } else if (arg == "--episodes") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseIntValue(value, options.episodes)) {
                cerr << "Invalid integer value for --episodes.\n";
                return false;
            }
        } else if (arg == "--max-steps") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseIntValue(value, options.maxStepsPerEpisode)) {
                cerr << "Invalid integer value for --max-steps.\n";
                return false;
            }
        } else if (arg == "--runs") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseIntValue(value, options.runs)) {
                cerr << "Invalid integer value for --runs.\n";
                return false;
            }
        } else if (arg == "--seed") {
            const char* value = requireValue(arg);
            if (value == nullptr || !parseUnsignedValue(value, options.seed)) {
                cerr << "Invalid integer value for --seed.\n";
                return false;
            }
        } else if (arg == "--epsilon-mode") {
            const char* value = requireValue(arg);
            if (value == nullptr) {
                return false;
            }
            const string mode = value;
            if (mode == "fixed") {
                options.epsilonMode = EpsilonMode::Fixed;
            } else if (mode == "linear") {
                options.epsilonMode = EpsilonMode::LinearDecay;
            } else {
                cerr << "Invalid epsilon mode: use fixed or linear.\n";
                return false;
            }
        } else if (arg == "--csv") {
            options.csv = true;
        } else if (arg == "--quiet") {
            options.quiet = true;
        } else {
            cerr << "Unknown argument: " << arg << '\n';
            return false;
        }
    }

    return validateOptions(options);
}

bool promptForProbabilities(TrainingOptions& options) {
    if (options.p1 < 0.0 || options.p2 < 0.0) {
        cout << "Enter p1 p2:\n";
        cin >> options.p1 >> options.p2;
    }

    if (!cin.good()) {
        cerr << "Failed to read p1 and p2.\n";
        return false;
    }

    if (options.p1 < 0.0 || options.p2 < 0.0 || options.p1 + options.p2 > 1.0) {
        cerr << "Invalid probabilities: require p1 >= 0, p2 >= 0, and p1 + p2 <= 1.\n";
        return false;
    }

    return true;
}

double epsilonForEpisode(const TrainingOptions& options, const int episodeIndex) {
    if (options.epsilonMode == EpsilonMode::Fixed || options.episodes <= 1) {
        return options.epsilon;
    }

    const double fraction = static_cast<double>(episodeIndex) / static_cast<double>(options.episodes - 1);
    return options.epsilon * max(0.0, 1.0 - fraction);
}

int uniformRandomAction(mt19937_64& rng) {
    uniform_int_distribution<int> dist(0, Gridworld::numActions() - 1);
    return dist(rng);
}

vector<int> greedyActions(const array<double, 4>& qValues, const double tolerance) {
    const double best = *max_element(qValues.begin(), qValues.end());
    vector<int> actions;
    for (int action = 0; action < Gridworld::numActions(); ++action) {
        if (best - qValues[action] <= tolerance) {
            actions.push_back(action);
        }
    }
    return actions;
}

int epsilonGreedyAction(const array<double, 4>& qValues, const double epsilon, mt19937_64& rng) {
    uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) < epsilon) {
        return uniformRandomAction(rng);
    }

    const vector<int> actions = greedyActions(qValues);
    uniform_int_distribution<int> tieBreaker(0, static_cast<int>(actions.size()) - 1);
    return actions[tieBreaker(rng)];
}

int randomGreedyAction(const array<double, 4>& qValues, mt19937_64& rng, const double tolerance) {
    const vector<int> actions = greedyActions(qValues, tolerance);
    uniform_int_distribution<int> tieBreaker(0, static_cast<int>(actions.size()) - 1);
    return actions[tieBreaker(rng)];
}

double expectedEpsilonGreedyValue(const array<double, 4>& qValues, const double epsilon) {
    const vector<int> greedy = greedyActions(qValues);
    const double greedyBonus = greedy.empty() ? 0.0 : (1.0 - epsilon) / static_cast<double>(greedy.size());
    const double exploratoryMass = epsilon / static_cast<double>(Gridworld::numActions());

    double expected = 0.0;
    for (int action = 0; action < Gridworld::numActions(); ++action) {
        double probability = exploratoryMass;
        if (find(greedy.begin(), greedy.end(), action) != greedy.end()) {
            probability += greedyBonus;
        }
        expected += probability * qValues[action];
    }
    return expected;
}

string epsilonModeName(const EpsilonMode mode) {
    return mode == EpsilonMode::Fixed ? "fixed" : "linear";
}

void printSummary(const string& algorithmName,
                  const TrainingOptions& options,
                  const TrainingSummary& summary,
                  const Gridworld& env) {
    const int overallEpisodes = summary.episodesPerRun * summary.runs;

    if (options.csv) {
        cout << fixed << setprecision(6)
             << algorithmName << ','
             << epsilonModeName(options.epsilonMode) << ','
             << options.alpha << ','
             << options.epsilon << ','
             << summary.episodesPerRun << ','
             << summary.runs << ','
             << overallEpisodes << ','
             << summary.totalSteps << ','
             << summary.totalTimeMs << ','
             << summary.meanNormalizedSteps << ','
             << summary.meanNormalizedTimeMs << '\n';
        return;
    }

    cout << "\n[" << algorithmName << "]\n";
    cout << fixed << setprecision(6);
    cout << "p1 = " << options.p1 << ", p2 = " << options.p2 << '\n';
    cout << "alpha = " << options.alpha
         << ", gamma = " << options.gamma
         << ", epsilon = " << options.epsilon
         << ", epsilon mode = " << epsilonModeName(options.epsilonMode) << '\n';
    cout << "Runs: " << summary.runs << '\n';
    cout << "Episodes per run: " << summary.episodesPerRun << '\n';
    cout << "Overall number of episodes: " << overallEpisodes << '\n';
    cout << "Overall number of time steps: " << summary.totalSteps << '\n';
    cout << "Computation time (ms): " << summary.totalTimeMs << '\n';
    cout << "Average normalized steps per episode: " << summary.meanNormalizedSteps << '\n';
    cout << "Average normalized time (ms) per episode: " << summary.meanNormalizedTimeMs << '\n';

    if (!options.quiet) {
        env.printPolicyFromQ(summary.averageQValues);
    }
}

void printUsage(const string& executableName, const string& algorithmName) {
    cout << "Usage: " << executableName << " [options]\n"
         << "Runs " << algorithmName << " on the 10x10 four-room gridworld.\n\n"
         << "Options:\n"
         << "  --p1 VALUE               Probability of moving in the chosen direction\n"
         << "  --p2 VALUE               Probability of staying in place\n"
         << "  --alpha VALUE            Step size (default: 0.1)\n"
         << "  --gamma VALUE            Discount factor (default: 0.9)\n"
         << "  --epsilon VALUE          Initial epsilon (default: 0.1)\n"
         << "  --epsilon-mode MODE      fixed or linear (default: fixed)\n"
         << "  --episodes N             Episodes per run (default: 25000)\n"
         << "  --max-steps N            Safety cap per episode (default: 10000)\n"
         << "  --runs N                 Number of independent runs to average (default: 1)\n"
         << "  --seed N                 Base RNG seed (default: 20260317)\n"
         << "  --csv                    Print a single CSV summary line\n"
         << "  --quiet                  Do not print the final policy\n\n"
         << "If --p1 and --p2 are omitted, the program prompts for them.\n"
         << "CSV columns:\n"
         << "algorithm,epsilon_mode,alpha,epsilon,episodes_per_run,runs,overall_episodes,"
            "total_steps,total_time_ms,avg_norm_steps,avg_norm_time_ms\n";
}
