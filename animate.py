from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# настройка
start_time = 0.0 
frames = 600
interval = 60

folder = Path(__file__).resolve().parent
data = [np.loadtxt(folder / name, delimiter=",", skiprows=1) for name in ["trajectory.csv", "trajectory_ab4.csv"]]
end_time = min(a[-1, 0] for a in data)

time = np.linspace(start_time, end_time, 6001)
x = [np.interp(time, a[:, 0], a[:, 1]) for a in data]
z = [np.interp(time, a[:, 0], a[:, 3]) for a in data]

fig, axes = plt.subplots(1, 2, figsize=(11, 5), sharex=True, sharey=True)
lines, dots = [], []
for i, name in enumerate(["DOPRI5", "AB4"]):
    axes[i].plot(x[i], z[i], alpha=0)
    line, = axes[i].plot([], [], linewidth=0.7, color="tab:blue")
    dot, = axes[i].plot([], [], "o", color="tab:red", markersize=4)
    axes[i].set(title=name, xlabel="x", ylabel="z")
    axes[i].grid(alpha=0.3)
    lines.append(line)
    dots.append(dot)
title = fig.suptitle("")
fig.tight_layout()


def update(frame):
    n = round(frame * (len(time) - 1) / (frames - 1))
    for i in range(2):
        lines[i].set_data(x[i][:n + 1], z[i][:n + 1])
        dots[i].set_data([x[i][n]], [z[i][n]])
    title.set_text(f"Цепь Чуа, τ = {time[n]:.2f}")


animation = FuncAnimation(fig, update, frames=frames, interval=interval, repeat=False)
plt.show()
