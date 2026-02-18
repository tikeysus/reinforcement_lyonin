## Question 2 — Learning Automata (LR-I and LR-P)

### How to Run

1. Compile and run:

```bash
    chmod +x run.sh
```

2. Compile and run:

```bash
    ./run.sh
```

2. Plot the differences of 3 algorithms(Optional) - You need to install pandas and matplotlib:

```bash
    python3 plot_results.py
```

4. The program:

- Runs UCB, LR-I, and LR-P on the same environments
- Prints results every 100 iterations
- Saves results to `results.csv`

---

### File Structure

- `Env.h / Env.cpp`
  Same bandit environment used for all algorithms.

- `UCB_Agent.h / UCB_Agent.cpp`
  UCB implementation (used for comparison).

- `LA_Agents.h / LA_Agents.cpp`
  Implements LR-I and LR-P learning automata.

- `Simulator.h / Simulator.cpp`
  Runs experiments for all three algorithms and logs results.

- `plot_results.py`
  Reads `results.csv` and generates comparison plots.

- `results.csv`
  Stores experiment results for plotting.

---

### Output

- Console output for each algorithm every 100 iterations
- CSV file used to generate plots for the report
