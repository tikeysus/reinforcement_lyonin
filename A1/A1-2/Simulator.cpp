#include <iostream>
#include "Simulator.h"
#include "Env.h"
#include "UCB_Agent.h"
#include "LA_Agents.h"
#include <fstream>

Simulator::Simulator(int worlds, int iterations, int arms,
                       double ucb_c,
                       double alpha, double beta) {
    this->worlds = worlds;
    this->iterations = iterations;
    this->arms = arms;
    this->ucb_c = ucb_c;
    this->alpha = alpha;
    this->beta = beta;

    srand(time(nullptr));
}

void Simulator::run() const {
    std::ofstream csv("./results.csv");
    csv << "world,algorithm,t,optimal_selected,average_reward\n";

    for (int w = 1; w <= this->worlds; w++) {
        std::cout << "=====================\nWorld: " << w << "\n";

        Env env(this->arms, rand());
        env.randomize();
        int best = env.optimalArm();

        // UCB
        {
            std::cout << "[UCB]\n";
            UCB_Agent agent(this->arms, this->ucb_c);

            int optimal_selected = 0;
            double total_reward = 0.0;

            for (int t = 1; t <= this->iterations; t++) {
                int action = agent.selectAction(t);
                int reward = env.pull(action);
                agent.update(action, reward);

                if (action == best) optimal_selected++;
                total_reward += reward;

                if (t % 100 == 0) {
                    double avg_reward = total_reward / (double)t;
                    std::cout << "Iteration: " << t
                              << " - Optimal selected: " << optimal_selected
                              << " - Avg reward: " << total_reward / (double)t
                              << "\n";
                    csv << w << ",UCB," << t << "," << optimal_selected << "," << avg_reward << "\n";
                }
            }
        }

        // LR-I
        {
            std::cout << "[LR-I]\n";
            LRI_Agent lri(this->arms, this->alpha, rand()); // seed for agent sampling

            int optimal_selected = 0;
            double total_reward = 0.0;

            for (int t = 1; t <= this->iterations; t++) {
                int action = lri.selectAction();
                int reward = env.pull(action);
                lri.update(action, reward);

                if (action == best) optimal_selected++;
                total_reward += reward;

                if (t % 100 == 0) {
                    double avg_reward = total_reward / (double)t;
                    std::cout << "Iteration: " << t
                              << " - Optimal selected: " << optimal_selected
                              << " - Avg reward: " << avg_reward
                              << "\n";
                    csv << w << ",LRI," << t << "," << optimal_selected << "," << avg_reward << "\n";
                }
            }
        }

        // LR-P
        {
            std::cout << "[LR-P]\n";
            LRP_Agent lrp(this->arms, this->alpha, this->beta, rand());

            int optimal_selected = 0;
            double total_reward = 0.0;

            for (int t = 1; t <= this->iterations; t++) {
                int action = lrp.selectAction();
                int reward = env.pull(action);
                lrp.update(action, reward);

                if (action == best) optimal_selected++;
                total_reward += reward;

                if (t % 100 == 0) {
                    double avg_reward = total_reward / (double)t;
                    std::cout << "Iteration: " << t
                              << " - Optimal selected: " << optimal_selected
                              << " - Avg reward: " << avg_reward
                              << "\n";
                    csv << w << ",LRP," << t << "," << optimal_selected << "," << avg_reward << "\n";
                }
            }
        }
    }

    csv.close();
}
