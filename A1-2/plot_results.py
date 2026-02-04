import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")


grouped = df.groupby(["algorithm", "t"]).mean().reset_index()

# Separate algorithms
ucb = grouped[grouped["algorithm"] == "UCB"]
lri = grouped[grouped["algorithm"] == "LRI"]
lrp = grouped[grouped["algorithm"] == "LRP"]

# Average Reward vs Time
plt.figure(figsize=(8, 5))
plt.plot(ucb["t"], ucb["average_reward"], label="UCB", linewidth=2)
plt.plot(lri["t"], lri["average_reward"], label="LR-I", linewidth=2)
plt.plot(lrp["t"], lrp["average_reward"], label="LR-P", linewidth=2)

plt.xlabel("Time steps")
plt.ylabel("Average reward")
plt.title("Average Reward vs Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("average_reward_vs_time.png", dpi=300)
plt.show()

# Optimal Action Selection vs Time
plt.figure(figsize=(8, 5))
plt.plot(ucb["t"], ucb["optimal_selected"], label="UCB", linewidth=2)
plt.plot(lri["t"], lri["optimal_selected"], label="LR-I", linewidth=2)
plt.plot(lrp["t"], lrp["optimal_selected"], label="LR-P", linewidth=2)

plt.xlabel("Time steps")
plt.ylabel("Optimal action selected (count)")
plt.title("Optimal Action Selection vs Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("optimal_action_vs_time.png", dpi=300)
plt.show()
