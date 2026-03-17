#include "Gridworld.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

Gridworld::Gridworld(const double p1, const double p2, const double gamma)
    : p1_(p1), p2_(p2), gamma_(gamma), transitionTable_(numStates()) {
    startStates_.reserve(numStates() - 1);
    for (int state = 0; state < numStates(); ++state) {
        if (!isTerminal(state)) {
            startStates_.push_back(state);
        }

        for (int action = 0; action < numActions(); ++action) {
            transitionTable_[state][action] = buildTransitions(state, action);
        }
    }
}

int Gridworld::numStates() const {
    return kWidth * kHeight;
}

int Gridworld::numActions() {
    return 4;
}

bool Gridworld::isTerminal(const int state) const {
    return state == goalState();
}

int Gridworld::goalState() const {
    return index(kWidth - 1, kHeight - 1);
}

double Gridworld::gamma() const {
    return gamma_;
}

pair<int, int> Gridworld::coordinates(const int state) const {
    return {state % kWidth, state / kWidth};
}

int Gridworld::manhattanDistanceToGoal(const int state) const {
    const auto [x, y] = coordinates(state);
    return (kWidth - 1 - x) + (kHeight - 1 - y);
}

int Gridworld::randomNonTerminalState(mt19937_64& rng) const {
    uniform_int_distribution<int> dist(0, static_cast<int>(startStates_.size()) - 1);
    return startStates_[dist(rng)];
}

const vector<Transition>& Gridworld::transitions(const int state, const int action) const {
    return transitionTable_[state][action];
}

pair<int, double> Gridworld::sampleTransition(const int state, const int action, mt19937_64& rng) const {
    const auto& options = transitions(state, action);
    uniform_real_distribution<double> dist(0.0, 1.0);
    const double draw = dist(rng);

    double cumulative = 0.0;
    for (const auto& option : options) {
        cumulative += option.probability;
        if (draw <= cumulative + 1e-12) {
            return {option.nextState, option.reward};
        }
    }

    const auto& last = options.back();
    return {last.nextState, last.reward};
}

void Gridworld::printPolicyFromQ(const vector<array<double, 4>>& qValues) const {
    cout << "\nOptimal greedy policy (G = goal):\n";

    for (int y = kHeight - 1; y >= 0; --y) {
        for (int x = 0; x < kWidth; ++x) {
            const int state = index(x, y);
            const string cell = isTerminal(state) ? "G" : greedyCellLabel(qValues[state]);
            cout << setw(6) << cell;
            if (x == kVerticalWallX) {
                cout << " |";
            }
        }
        cout << '\n';

        if (y == kHorizontalWallY + 1) {
            cout << string(70, '-') << '\n';
        }
    }
}

string Gridworld::actionName(const int action) {
    switch (action) {
        case UP:
            return "up";
        case DOWN:
            return "down";
        case RIGHT:
            return "right";
        case LEFT:
            return "left";
        default:
            return "?";
    }
}

bool Gridworld::inGrid(const int x, const int y) const {
    return x >= 0 && x < kWidth && y >= 0 && y < kHeight;
}

int Gridworld::index(const int x, const int y) const {
    return y * kWidth + x;
}

bool Gridworld::crossesWall(const int x1, const int y1, const int x2, const int y2) const {
    if (y1 == y2) {
        const bool crossesVerticalWall =
            (x1 == kVerticalWallX && x2 == kVerticalWallX + 1) ||
            (x2 == kVerticalWallX && x1 == kVerticalWallX + 1);
        if (crossesVerticalWall && y1 != kTopDoorY && y1 != kBottomDoorY) {
            return true;
        }
    }

    if (x1 == x2) {
        const bool crossesHorizontalWall =
            (y1 == kHorizontalWallY && y2 == kHorizontalWallY + 1) ||
            (y2 == kHorizontalWallY && y1 == kHorizontalWallY + 1);
        if (crossesHorizontalWall && x1 != kLeftDoorX && x1 != kRightDoorX) {
            return true;
        }
    }

    return false;
}

pair<int, int> Gridworld::moveOrStay(const int x, const int y, const int dx, const int dy) const {
    const int nx = x + dx;
    const int ny = y + dy;

    if (!inGrid(nx, ny) || crossesWall(x, y, nx, ny)) {
        return {x, y};
    }
    return {nx, ny};
}

vector<Transition> Gridworld::buildTransitions(const int state, const int action) const {
    if (isTerminal(state)) {
        return {{state, 1.0, 0.0}};
    }

    const auto [x, y] = coordinates(state);

    array<pair<int, int>, 3> deltas{};
    switch (action) {
        case UP:
            deltas = {{{0, 1}, {-1, 0}, {1, 0}}};
            break;
        case DOWN:
            deltas = {{{0, -1}, {-1, 0}, {1, 0}}};
            break;
        case RIGHT:
            deltas = {{{1, 0}, {0, 1}, {0, -1}}};
            break;
        case LEFT:
            deltas = {{{-1, 0}, {0, 1}, {0, -1}}};
            break;
        default:
            break;
    }

    double sideMass = 1.0 - p1_ - p2_;
    if (sideMass < 0.0 && sideMass > -1e-12) {
        sideMass = 0.0;
    }
    const double sideProbability = sideMass / 2.0;

    array<double, kWidth * kHeight> probabilityByState{};
    probabilityByState[state] += p2_;

    const auto [tx, ty] = moveOrStay(x, y, deltas[0].first, deltas[0].second);
    probabilityByState[index(tx, ty)] += p1_;

    const auto [sx1, sy1] = moveOrStay(x, y, deltas[1].first, deltas[1].second);
    probabilityByState[index(sx1, sy1)] += sideProbability;

    const auto [sx2, sy2] = moveOrStay(x, y, deltas[2].first, deltas[2].second);
    probabilityByState[index(sx2, sy2)] += sideProbability;

    vector<Transition> out;
    out.reserve(4);
    for (int nextState = 0; nextState < numStates(); ++nextState) {
        const double probability = probabilityByState[nextState];
        if (probability <= numeric_limits<double>::epsilon()) {
            continue;
        }

        const double reward = nextState == goalState() ? 500.0 : -1.0;
        out.push_back({nextState, probability, reward});
    }

    return out;
}

string Gridworld::greedyCellLabel(const array<double, 4>& qValues) const {
    const double best = *max_element(qValues.begin(), qValues.end());
    string cell;

    if (fabs(qValues[UP] - best) <= 1e-6) {
        cell += "U";
    }
    if (fabs(qValues[DOWN] - best) <= 1e-6) {
        cell += "D";
    }
    if (fabs(qValues[RIGHT] - best) <= 1e-6) {
        cell += "R";
    }
    if (fabs(qValues[LEFT] - best) <= 1e-6) {
        cell += "L";
    }

    return cell.empty() ? "." : cell;
}
