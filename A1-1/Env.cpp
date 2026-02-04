#include "Env.h"
#include <algorithm>

Env::Env(int k, int seed) {
    this->k = k;
    this->rng = std::mt19937(seed);
    this->uni = std::uniform_real_distribution<double>(0.0, 1.0);
    this->q.resize(k);
}

void Env::randomize() {
    for (int i = 0; i < this->k; i++) {
        this->q[i] = this->uni(this->rng);
    }
}

int Env::pull(int action) {
    if (this->q[action] > this->uni(this->rng)) {
        return 1;
    }
    return 0;
}

int Env::optimalArm() {
    return int(std::max_element(this->q.begin(), this->q.end()) - this->q.begin());
}