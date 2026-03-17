#include "Gridworld.h"
#include "MonteCarloControl.h"
#include "RLCommon.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        const string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0], "Monte Carlo on-policy control");
            return 0;
        }
    }

    TrainingOptions options;
    if (!parseCommonArguments(argc, argv, options)) {
        printUsage(argv[0], "Monte Carlo on-policy control");
        return 1;
    }
    if (!promptForProbabilities(options)) {
        return 1;
    }

    const Gridworld env(options.p1, options.p2, options.gamma);
    MonteCarloControl mc(env, options);
    const TrainingSummary summary = mc.run();
    printSummary("Monte Carlo Control", options, summary, env);
    return 0;
}
