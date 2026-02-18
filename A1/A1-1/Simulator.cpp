#include <iostream>
#include "Simulator.h"
#include "Env.h"
#include "Agent.h"


Simulator::Simulator(int worlds, int iterations, int arms, int c) {
    this->worlds = worlds;
    this->iterations = iterations;
    this->arms = arms;
    this->c = c;
    srand(time(nullptr));
}

void Simulator::run() const {
    for (int w = 1; w <= worlds; w++) {
        std::cout << "=====================\nWorld: " << w << '\n';
        Env env(this->arms, rand());
        Agent agent(this->arms, this->c);
        env.randomize();

        int optimized_action_selected = 0;
        double total_reward = 0.0f;
        for (int t = 1; t <= this->iterations; t++) {
            int cur_action = agent.selectAction(t);
            int cur_reward = env.pull(cur_action);
            agent.update(cur_action, cur_reward);

            if (env.optimalArm() == cur_action) {
                optimized_action_selected++;
            }

            total_reward += cur_reward;

            if (t % 100 == 0) {
                std::cout << "Iteration: " << t << " - Optimized action selected: "
                    << optimized_action_selected << " - Average reward: " << total_reward / static_cast<double>(t) << '\n';
            }
        }
    }
}
