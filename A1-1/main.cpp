#include "Simulator.h"

int main() {
    const int Runs = 100, T = 5000, k = 10;
    const double c = 2.0f;
    Simulator sim(Runs, T, k, c);
    sim.run();
    return 0;
}
