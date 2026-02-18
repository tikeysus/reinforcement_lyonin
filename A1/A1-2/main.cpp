#include "Simulator.h"

int main() {
    Simulator sim(100, 5000, 10, 2.0, 0.1, 0.1);
    sim.run();
    return 0;
}