## Assignment 3

## Files Included

```text
mc.cpp / MonteCarloControl.*     - Monte Carlo on-policy control with epsilon-soft policy
sarsa.cpp / Sarsa.*              - SARSA implementation
q.cpp / QLearning.*              - Q-learning implementation
expected.cpp / ExpectedSarsa.*   - Expected SARSA implementation
doubleq.cpp / DoubleQLearning.*  - Double Q-learning implementation
Gridworld.*                      - 10x10 four-room gridworld environment
RLCommon.*                       - shared CLI parsing, statistics, and printing helpers
reportA3.pdf                     - short report and comparison
```

## Compilation

```bash
mkdir build
cd build
cmake ..
make
```

This generates five executables:

```text
mc
sarsa
q
expected
doubleq
```

## Running the Programs

Interactive mode:

```bash
./mc
./sarsa
./q
./expected
./doubleq
```

Each program asks for:

```text
p1 p2
```

Example:

```text
1 0
```

You can also supply parameters on the command line:

```bash
./q --p1 1.0 --p2 0.0 --alpha 0.1 --epsilon 0.1 --episodes 25000
```

Useful options:

```text
--alpha VALUE
--epsilon VALUE
--epsilon-mode fixed|linear
--episodes N
--runs N
--seed N
--csv
--quiet
```

To reproduce the report table after building:

```bash
./run.sh
```

This regenerates `experiment_results.csv` for all five algorithms using `p1 = 1`, `p2 = 0`, three `alpha`
values, and both fixed and linearly decaying epsilon schedules.

## Output

Each executable prints:

- overall number of episodes
- overall number of time steps
- computation time
- average normalized steps per episode
- average normalized time per episode
- final greedy policy
