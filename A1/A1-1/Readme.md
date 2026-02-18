## Question 1 — UCB Algorithm

### How to Run

1. Compile and run:

```bash
    chmod +x run.sh
```

2. Compile and run:

```bash
    ./run.sh
```

3. The program:

- Runs UCB for multiple environments
- Prints results every 100 iterations

---

### File Structure

- `Env.h / Env.cpp`
  Defines the k-armed bandit environment and reward generation.

- `Agent.h / Agent.cpp`
  Implements the UCB learning algorithm.

- `Simulator.h / Simulator.cpp`
  Controls the experiment:
  - Runs multiple environments
  - Tracks performance
  - Prints results

- `main.cpp`
  Creates a `Simulator` instance and starts the experiment.

---

### Output

- Console output every 100 iterations
- CSV file with average reward and optimal action counts
