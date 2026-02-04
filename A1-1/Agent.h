#ifndef A1_AGENT_H
#define A1_AGENT_H

#include <vector>

class Agent {
public:
    Agent(int k, double c);

    // returns an arm index
    int selectAction(int t);
    // updates Q and N
    void update(int action, double reward);

private:
    std::vector<double> Q;
    std::vector<int> N;
    double c;
    int k;

    // helpers
    int findNeverExplored();
    int findHighestScore(int t);
};


#endif