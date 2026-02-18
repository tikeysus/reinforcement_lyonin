#include "Env.h"
#include <algorithm>

Env::Env(int k, int seed)
    : k(k), q(k), rng(seed), uni(0.0, 1.0) {}

void Env::randomize() {
    for (int i = 0; i < this->k; i++) {
        this->q[i] = this->uni(this->rng);
    }
}

int Env::pull(int action) {
    return (this->uni(this->rng) < this->q[action]) ? 1 : 0;
}

int Env::optimalArm() const {
    return int(std::max_element(this->q.begin(), this->q.end()) - this->q.begin());
}
