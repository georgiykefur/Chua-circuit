from pathlib import Path
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

matplotlib.use("Agg") 

folder = Path(__file__).resolve().parent
plots = folder / "plots"
plots.mkdir(exist_ok=True)
names = ["DOPRI5", "AB4"]
files = ["trajectory.csv", "trajectory_ab4.csv"]

# в CSV четыре столбца: tau, x, y, z.
fig_time, axes = plt.subplots(3, 2, figsize=(12, 8), sharex=True)
fig_phase, phase_axes = plt.subplots(1, 2, figsize=(12, 5))
fig_3d = plt.figure(figsize=(12, 5))

for column in range(2):
    data = np.loadtxt(folder / files[column], delimiter=",", skiprows=1, ndmin=2)
    if len(data) < 2 or not np.isfinite(data).all():
        raise ValueError("Нет корректной траектории: " + files[column])
    tau, x, y, z = data.T

    for row, (label, values) in enumerate(zip(["x", "y", "z"], [x, y, z])):
        ax = axes[row, column]
        ax.plot(tau, values, linewidth=0.6)
        ax.set_ylabel(label)
        ax.grid(alpha=0.3)
    axes[0, column].set_title(names[column])
    axes[2, column].set_xlabel("τ")

    ax = phase_axes[column]
    ax.plot(x, z, linewidth=0.5)
    ax.set(xlabel="x", ylabel="z", title=names[column])
    ax.grid(alpha=0.3)

    ax = fig_3d.add_subplot(1, 2, column + 1, projection="3d")
    ax.plot(x, y, z, linewidth=0.4)
    ax.set(xlabel="x", ylabel="y", zlabel="z", title=names[column])

for fig, name in [(fig_time, "time_series"), (fig_phase, "phase_portraits"),
                  (fig_3d, "phase_3d")]:
    fig.tight_layout()
    fig.savefig(plots / (name + ".png"), dpi=160)
    plt.close(fig)
def plot_lyapunov():
    fig, ax = plt.subplots(figsize=(9, 4))
    for name, filename in [("DOPRI5", "lyapunov.csv"), ("AB4", "lyapunov_ab4.csv")]:
        data = np.loadtxt(folder / filename, delimiter=",", skiprows=1, ndmin=2)
        ax.plot(data[:, 1], data[:, 2], label=name)

    ax.axhline(0, color="black", linewidth=0.7)
    ax.set(xlabel="Время измерения", ylabel="λ(T)",
           title="Сходимость показателя Ляпунова")
    ax.legend()
    ax.grid(alpha=0.3)
    fig.tight_layout()
    fig.savefig(plots / "lyapunov.png", dpi=160)
    plt.close(fig)


plot_lyapunov()
print("Графики сохранены в папке plots")
