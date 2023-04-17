import re
import csv
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.dates as mpl_dates
from datetime import datetime
import warnings

warnings.filterwarnings("ignore", category=RuntimeWarning)


def load_data(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        data = file.read()
    return data


def normalize_line_endings(data):
    return data.replace("\r\n", "\n")


def extract_data(data):
    pattern = r"(\d{2}:\d{2}:\d{2})\nLight Level: (\d+) Ohms\nDistance: ([\d.]+)cm\nTemperature: ([\d.]+)°C\nAcceleration X: ([-\d.]+)g\nAcceleration Y: ([-\d.]+)g\nAcceleration Z: ([-\d.]+)g\nCompleted stage: ([\w\s]+)\nNext stage: ([\w\s]+)\n={10,}"
    matches = re.findall(pattern, data)
    return matches


def write_csv(matches, file_name="table.csv"):
    with open(file_name, "w", newline="", encoding="utf-8") as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["Timestamp", "Light Level (Ohms)", "Distance (cm)",
                            "Temperature (°C)", "Acc X (g)", "Acc Y (g)", "Acc Z (g)", "Completed Stage", "Next Stage"])
        for match in matches:
            csvwriter.writerow(match)


def process_matches(matches):
    timestamps = []
    light_levels = []
    distances = []
    temperatures = []
    acc_x = []
    acc_y = []
    acc_z = []
    completed_stages = []
    next_stages = []

    for match in matches:
        timestamp = datetime.strptime(match[0], "%H:%M:%S")
        timestamps.append(timestamp)

        light_level = int(match[1])
        light_levels.append(light_level)

        distance = float(match[2])
        distances.append(distance)

        temperature = float(match[3])
        temperatures.append(temperature)

        acc_x_val = float(match[4]) * 0.76
        acc_x.append(acc_x_val)

        acc_y_val = float(match[5]) * 0.76
        acc_y.append(acc_y_val)

        acc_z_val = float(match[6]) * 0.76
        acc_z.append(acc_z_val)

        completed_stages.append(match[7])
        next_stages.append(match[8])

    return timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z, completed_stages, next_stages


def main():
    # Load and process data
    data = load_data("D:\\DATA.TXT")
    data = normalize_line_endings(data)
    matches = extract_data(data)

    if matches:
        write_csv(matches)
        print("Data has been saved to table.csv.")

        (timestamps, light_levels, distances, temperatures,
         acc_x, acc_y, acc_z, completed_stages, next_stages) = process_matches(matches)

        # Visualization functions
        plot_main_graphs(timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z)
        plot_stage_bar_charts(distances, completed_stages, next_stages)
        plot_extra_visualizations(timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z)
        plot_correlation_matrix(light_levels, distances, temperatures, acc_x, acc_y, acc_z)

    else:
        print("No data found.")


def plot_main_graphs(timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z):
    fig, ax = plt.subplots(5, 1, figsize=(12, 20), sharex=True)
    ax[0].plot(timestamps, light_levels)
    ax[0].set_title("Light Levels")
    ax[0].set_ylabel("Ohms")

    ax[1].plot(timestamps, distances)
    ax[1].set_title("Distances")
    ax[1].set_ylabel("cm")

    ax[2].plot(timestamps, temperatures)
    ax[2].set_title("Temperatures")
    ax[2].set_ylabel("°C")

    ax[3].plot(timestamps, acc_x, label="X")
    ax[3].plot(timestamps, acc_y, label="Y")
    ax[3].plot(timestamps, acc_z, label="Z")
    ax[3].set_title("Acceleration")
    ax[3].set_ylabel("g")
    ax[3].legend()

    ax[4].xaxis.set_major_formatter(mpl_dates.DateFormatter('%H:%M:%S'))
    plt.xticks(rotation=45)

    plt.tight_layout()
    # plt.show()

    plt.tight_layout()
    plt.savefig("main_graphs.png")
    plt.close(fig)
    print("Main graphs have been saved to main_graphs.png.")


def plot_stage_bar_charts(distances, completed_stages, next_stages):
    completed_stage_counts = {stage: completed_stages.count(stage) for stage in set(completed_stages)}
    next_stage_counts = {stage: next_stages.count(stage) for stage in set(next_stages)}

    fig, ax = plt.subplots(1, 2, figsize=(12, 6))
    ax[0].bar(completed_stage_counts.keys(), completed_stage_counts.values())
    ax[0].set_title("Completed Stages")
    ax[0].set_ylabel("Counts")

    ax[1].bar(next_stage_counts.keys(), next_stage_counts.values())
    ax[1].set_title("Next Stages")
    ax[1].set_ylabel("Counts")

    plt.tight_layout()
    # plt.show()

    plt.tight_layout()
    plt.savefig("stage_bar_charts.png")
    plt.close(fig)
    print("Stage bar charts saved to stage_bar_charts.png")

def plot_extra_visualizations(timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z):
    # Add any extra visualization functions here
    pass


def plot_correlation_matrix(light_levels, distances, temperatures, acc_x, acc_y, acc_z):
    data = np.array([light_levels, distances, temperatures, acc_x, acc_y, acc_z]).T
    corr_matrix = np.corrcoef(data, rowvar=False)
    sns.heatmap(corr_matrix, annot=True, cmap="coolwarm", xticklabels=["Light", "Distance", "Temperature", "Acc X", "Acc Y", "Acc Z"], yticklabels=["Light", "Distance", "Temperature", "Acc X", "Acc Y", "Acc Z"])
    plt.title("Correlation Matrix")
    plt.savefig("correlation_matrix.png")
    plt.close()
    print("Correlation matrix saved to correlation_matrix.png")


if __name__ == "__main__":
    main()

