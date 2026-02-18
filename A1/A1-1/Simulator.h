#ifndef A1_SIMULATOR_H
#define A1_SIMULATOR_H


class Simulator {
public:
    Simulator(int worlds, int iterations, int arms, int c);
    void run() const;
private:
    int worlds, iterations, arms;
    double c;
};


#endif