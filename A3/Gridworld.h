//
// Created by Arshia on 2026-03-17.
//

#ifndef A3_3_GRIDWORLD_H
#define A3_3_GRIDWORLD_H

#include <array>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct Transition {
    int nextState;
    double probability;
    double reward;
};

class Gridworld {
public:
    enum Action { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };

    Gridworld(double p1, double p2, double gamma);

    int numStates() const;
    static int numActions();
    bool isTerminal(int state) const;
    int goalState() const;
    double gamma() const;

    std::pair<int, int> coordinates(int state) const;
    int manhattanDistanceToGoal(int state) const;
    int randomNonTerminalState(std::mt19937_64& rng) const;

    const std::vector<Transition>& transitions(int state, int action) const;
    std::pair<int, double> sampleTransition(int state, int action, std::mt19937_64& rng) const;

    void printPolicyFromQ(const std::vector<std::array<double, 4>>& qValues) const;
    static std::string actionName(int action);

private:
    static constexpr int kWidth = 10;
    static constexpr int kHeight = 10;
    static constexpr int kVerticalWallX = 4;
    static constexpr int kHorizontalWallY = 4;
    static constexpr int kTopDoorY = 7;
    static constexpr int kBottomDoorY = 2;
    static constexpr int kLeftDoorX = 2;
    static constexpr int kRightDoorX = 7;

    double p1_;
    double p2_;
    double gamma_;

    std::vector<std::array<std::vector<Transition>, 4>> transitionTable_;
    std::vector<int> startStates_;

    bool inGrid(int x, int y) const;
    int index(int x, int y) const;
    bool crossesWall(int x1, int y1, int x2, int y2) const;
    std::pair<int, int> moveOrStay(int x, int y, int dx, int dy) const;
    std::vector<Transition> buildTransitions(int state, int action) const;
    std::string greedyCellLabel(const std::array<double, 4>& qValues) const;
};

#endif
