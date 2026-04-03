"""Строит график времени сортировок vs размер массива.

Читает results/timings.csv (выход бенчмарка), пишет results/plots.png.
Bubble-колонки -1 трактуются как 'пропущено' и не отображаются.
"""
import csv
import sys
from pathlib import Path

import matplotlib.pyplot as plt

CSV = Path("results/timings.csv")
PNG = Path("results/plots.png")


def load(path):
    rows = []
    with open(path, newline="") as f:
        for r in csv.DictReader(f):
            rows.append({k: float(v) for k, v in r.items()})
    rows.sort(key=lambda r: r["n"])
    return rows


def main():
    if not CSV.exists():
        sys.exit(f"Нет {CSV}. Сначала: make bench")
    rows = load(CSV)
    n = [r["n"] for r in rows]

    series = {
        "Пузырьковая":  [r["bubble_ms"]   for r in rows],
        "Быстрая":      [r["quick_ms"]    for r in rows],
        "Слиянием":     [r["merge_ms"]    for r in rows],
        "std::sort":    [r["std_sort_ms"] for r in rows],
    }

    fig, ax = plt.subplots(figsize=(10, 6))
    for name, ys in series.items():
        xs = [x for x, y in zip(n, ys) if y >= 0]
        ys = [y for y in ys if y >= 0]
        ax.plot(xs, ys, marker="o", label=name)
    ax.set_xlabel("Размер массива N")
    ax.set_ylabel("Время сортировки, мс")
    ax.set_title("ЛР1, вариант 5: сравнение сортировок")
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.grid(True, which="both", linestyle="--", alpha=0.4)
    ax.legend()
    fig.tight_layout()
    fig.savefig(PNG, dpi=130)
    print(f"Сохранено: {PNG}")


if __name__ == "__main__":
    main()
