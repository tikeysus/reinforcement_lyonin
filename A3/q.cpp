#include "Gridworld.h"
#include "QLearning.h"
#include "RLCommon.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        const string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0], "Q-learning");
            return 0;
        }
    }

    TrainingOptions options;
    if (!parseCommonArguments(argc, argv, options)) {
        printUsage(argv[0], "Q-learning");
        return 1;
    }
    if (!promptForProbabilities(options)) {
        return 1;
    }

    const Gridworld env(options.p1, options.p2, options.gamma);
    QLearning qLearning(env, options);
    const TrainingSummary summary = qLearning.run();
    printSummary("Q-learning", options, summary, env);
    return 0;
}
