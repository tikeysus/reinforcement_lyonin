#ifndef UCB_AGENT_H
#define UCB_AGENT_H

#include <vector>

class UCB_Agent {
public:
    UCB_Agent(int k, double c);

    int selectAction(int t);
    void update(int action, int reward);

private:
    std::vector<double> Q;
    std::vector<int> N;
    double c;
    int k;

    int findNeverExplored();
    int findHighestScore(int t);
};

#endif
