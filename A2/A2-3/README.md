## Question3 - VIA VS PIA

## Files Included

```
main.cpp              – program entry point
via.*                 – Value Iteration runner
pia.*                 – Policy Iteration runner
ValueIteration.*      – value iteration implementation
PolicyIteration.*     – policy iteration implementation
GridWorld.*           – Gridworld MDP environment
Report3.pdf           – short report and comparison
```

## Compilation

Example:

```
mkdir build
cd build
cmake ..
make
```

This generates the executable:

```
A2_3
```

## Running the Program

Run:

```
./A2_3
```

or just run:

1. Give access to runner

```bash
    chmod +x run.sh
```

2. Compile and run:

```bash
    ./run.sh
```

You will be asked to choose:

```
1) Value Iteration
2) Policy Iteration
3) Both
```

Then enter environment parameters:

```
p1 p2 rup rdown rright rleft
```

Example (deterministic Example-4.1-style setup):

```
1 0 -1 -1 -1 -1
```

## Output

Both algorithms print:

- Number of iterations
- Computation time per iteration
- Optimal greedy policy derived from the final value function
