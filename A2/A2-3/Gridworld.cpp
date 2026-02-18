#include "Gridworld.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

Gridworld::Gridworld(int N, double p1, double p2, double rup, double rdown, double rright, double rleft,
    double gamma, double theta) : N(N), S(N*N), p1(p1), p2(p2), gamma(gamma), theta(theta)
{
    r[UP] = rup;
    r[DOWN] = rdown;
    r[RIGHT] = rright;
    r[LEFT] = rleft;

    terminal.assign(S, false);
    terminal[idx(0,0)] = true;
    terminal[idx(N-1,N-1)] = true;
}

int Gridworld::numStates() const { return S; }
int Gridworld::numActions() { return 4; }
bool Gridworld::isTerminal(int s) const { return terminal[s]; }

double Gridworld::getGamma() const { return gamma; }
double Gridworld::getTheta() const { return theta; }

bool Gridworld::inGrid(int x, int y) const {
    return x >= 0 && x < N && y >= 0 && y < N;
}
int Gridworld::idx(int x, int y) const { return x*N + y; }
pair<int,int> Gridworld::xy(int s) const { return {s / N, s % N}; }

vector<Transition> Gridworld::transitions(int s, int a) const {
    vector<Transition> out;

    // terminal self-loop with reward 0
    if (isTerminal(s)) {
        out.push_back({s, 1.0, 0.0});
        return out;
    }

    auto [x,y] = xy(s);

    // intended move delta for action a
    int dx = 0, dy = 0;
    if (a == UP) dx = -1;
    else if (a == DOWN) dx = 1;
    else if (a == RIGHT) dy = 1;
    else if (a == LEFT) dy = -1;

    int tx = x + dx;
    int ty = y + dy;

    bool intendedOff = !inGrid(tx, ty);
    int intended = intendedOff ? s : idx(tx, ty);

    array<pair<int,int>,2> sideCoords;
    if (a == UP || a == DOWN) {
        sideCoords = { make_pair(tx, ty-1), make_pair(tx, ty+1) };
    } else {
        sideCoords = { make_pair(tx-1, ty), make_pair(tx+1, ty) };
    }

    auto sideState = [&](int sx, int sy) -> int {
        // off-grid => stay
        if (!inGrid(sx, sy)) return s;
        return idx(sx, sy);
    };

    int side1 = sideState(sideCoords[0].first, sideCoords[0].second);
    int side2 = sideState(sideCoords[1].first, sideCoords[1].second);

    double pSlip = 1.0 - p1 - p2;
    if (pSlip < 0.0) pSlip = 0.0;

    // Reward depends only on action
    double rew = r[a];

    // Probability mass for "stay"
    // If intended off-grid: stay with p1+p2 (as spec)
    // Else: stay with p2
    double pStay = intendedOff ? (p1 + p2) : p2;

    // Intended probability is p1 only if intended in-grid, else 0
    double pIntended = intendedOff ? 0.0 : p1;

    // build transitions (may have duplicate next states, merge later)
    out.push_back({intended, pIntended, rew});
    out.push_back({s,pStay,rew});
    out.push_back({side1,0.5*pSlip,rew});
    out.push_back({side2,0.5*pSlip,rew});

    sort(out.begin(), out.end(), [](const Transition& a, const Transition& b){
        return a.s2 < b.s2;
    });

    vector<Transition> merged;
    for (const auto& t : out) {
        if (t.prob <= 0.0) continue;
        if (!merged.empty() && merged.back().s2 == t.s2) merged.back().prob += t.prob;
        else merged.push_back(t);
    }
    return merged;
}

void Gridworld::printPolicyGreedyFromV(const vector<double>& V) const {
    cout << "\nOptimal policy (greedy from V):\n";
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            int s = idx(x,y);
            if (isTerminal(s)) {
                cout << setw(6) << "T";
                continue;
            }
            array<double,4> Q{};
            for (int a = 0; a < 4; a++) {
                double q = 0.0;
                for (const auto& t : transitions(s,a)) {
                    q += t.prob * (t.reward + gamma * V[t.s2]);
                }
                Q[a] = q;
            }
            double mx = *max_element(Q.begin(), Q.end());
            string cell;
            if (fabs(Q[UP]-mx) < 1e-12) cell += "U";
            if (fabs(Q[DOWN]-mx) < 1e-12) cell += "D";
            if (fabs(Q[RIGHT]-mx) < 1e-12) cell += "R";
            if (fabs(Q[LEFT]-mx) < 1e-12) cell += "L";
            cout << setw(6) << cell;
        }
        cout << "\n";
    }
}
