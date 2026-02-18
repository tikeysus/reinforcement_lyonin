#ifndef A1_SIMULATOR_H
#define A1_SIMULATOR_H

class Simulator {
public:
    Simulator(int worlds, int iterations, int arms,
               double ucb_c,
               double alpha, double beta);

    void run() const;

private:
    int worlds, iterations, arms;
    double ucb_c;
    double alpha, beta;
};

#endif
