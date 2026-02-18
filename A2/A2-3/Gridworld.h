//
// Created by Arshia on 2026-02-17.
//

#ifndef A2_3_GRIDWORLD_H
#define A2_3_GRIDWORLD_H

#include <vector>
#include <array>

struct Transition {
    // next state index
    int s2;
    // transition probability
    double prob;
    // immediate reward
    double reward;
};

class Gridworld {
public:
    enum Action { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };

    Gridworld(int N,
                 double p1, double p2,
                 double rup, double rdown, double rright, double rleft,
                 double gamma, double theta);

    int numStates() const;

    static int numActions();
    bool isTerminal(int s) const;

    double getGamma() const;
    double getTheta() const;

    // transition model per assignment
    std::vector<Transition> transitions(int s, int a) const;

    // printing helpers
    void printPolicyGreedyFromV(const std::vector<double>& V) const;

private:
    int N;
    int S;
    double p1, p2;
    std::array<double,4> r{};
    double gamma, theta;
    std::vector<bool> terminal;

    bool inGrid(int x, int y) const;
    int idx(int x, int y) const;
    std::pair<int,int> xy(int s) const;
};


#endif //A2_3_GRIDWORLD_H