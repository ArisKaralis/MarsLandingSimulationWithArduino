import re
import csv
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.dates as mpl_dates
from datetime import datetime
import warnings
import pandas as pd

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


def write_csv(matches, angles, file_name):
    with open(file_name, "w", newline="", encoding="utf-8") as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["Timestamp", "Light Level (Ohms)", "Distance (cm)",
                            "Temperature (°C)", "Acc X (g)", "Acc Y (g)", "Acc Z (g)", "Completed Stage", "Next Stage", "Angle (degrees)"])
        for i, match in enumerate(matches):
            csvwriter.writerow(match + (angles[i],))


def calculate_angle(ax, ay):
    angle = np.arctan2(ay, ax)
    angle_degrees = np.degrees(angle)
    return angle_degrees


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
    angles = []
    G = 9.81

    for match in matches:
        timestamp = datetime.strptime(match[0], "%H:%M:%S")
        timestamps.append(timestamp)

        light_level = int(match[1])
        light_levels.append(light_level)

        distance = float(match[2])
        distances.append(distance)

        temperature = float(match[3])
        temperatures.append(temperature)

        acc_x_val = float(match[4]) * G
        acc_x.append(acc_x_val)

        acc_y_val = float(match[5]) * G
        acc_y.append(acc_y_val)

        acc_z_val = float(match[6]) * G
        acc_z.append(acc_z_val)

        completed_stages.append(match[7])
        next_stages.append(match[8])

     # Calculate the angle and append it to the angles list
        angle = calculate_angle(acc_x_val, acc_y_val)
        angles.append(angle)

    # Return tuple
    return timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z, completed_stages, next_stages, angles


def calculate_power_and_energy(light_levels, timestamps):
    efficiency_constant = 0.20
    area = 1

    power_kw = [L * efficiency_constant *
                area / 1000 for L in light_levels]  # kW

    # Calculate time intervals in hours
    time_intervals_hours = [
        (timestamps[i + 1] - timestamps[i]).total_seconds() / 3600
        for i in range(len(timestamps) - 1)
    ]

    energy_kwh = [power_kw[i] * time_intervals_hours[i]
                  for i in range(len(power_kw) - 1)]  # kWh
    return power_kw, energy_kwh


def plot_kwh_t_kw_t_graphs(timestamps, energy_kwh, power_kw):
    fig, ax = plt.subplots(2, 1, figsize=(12, 8), sharex=True)
    ax[0].plot(timestamps[1:], energy_kwh)
    ax[0].set_title("Cumulative Energy vs Time")
    ax[0].set_ylabel("Energy (kWh)")

    ax[1].plot(timestamps, power_kw)
    ax[1].set_title("Power vs Time")
    ax[1].set_ylabel("Power (kW)")
    ax[1].set_xlabel("Time")

    ax[1].xaxis.set_major_formatter(mpl_dates.DateFormatter('%H:%M:%S'))

    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig("kwh_t_kw_t_graphs.png")
    plt.close(fig)
    
    print("Cumulative Energy vs Time and Power vs Time graphs saved to kwh_t_kw_t_graphs.png.")


def plot_angles_t_graph(timestamps, angles):
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.plot(timestamps, angles)
    ax.set_title("Angle vs Time")
    ax.set_ylabel("Angle (degrees)")
    ax.set_xlabel("Time")

    ax.xaxis.set_major_formatter(mpl_dates.DateFormatter('%H:%M:%S'))

    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig("angle_t_graph.png")
    plt.close(fig)

    print("Angle vs Time graph saved to angle_t_graph.png.")


def main():
    # Load and process data
    data = load_data("D:\\DATA.TXT")
    data = normalize_line_endings(data)
    matches = extract_data(data)
    file_name = "table.csv"

    if matches:
        (timestamps, light_levels, distances, temperatures,
         acc_x, acc_y, acc_z, completed_stages, next_stages, angles) = process_matches(matches)

        power_kw, energy_kwh = calculate_power_and_energy(
            light_levels, timestamps)

        write_csv(matches, angles, file_name)
        print("Data has been saved to table.csv.")

        # Visualization functions
        plot_main_graphs(timestamps, light_levels, distances,
                         temperatures, acc_x, acc_y, acc_z, angles)
        plot_stage_bar_charts(distances, completed_stages, next_stages)
        plot_correlation_matrix(light_levels, distances,
                                temperatures, acc_x, acc_y, acc_z)

        plot_kwh_t_kw_t_graphs(timestamps, energy_kwh, power_kw)

        plot_angles_t_graph(timestamps, angles)

    else:
        print("No data found.")


def plot_main_graphs(timestamps, light_levels, distances, temperatures, acc_x, acc_y, acc_z, angles):
    
    fig, ax = plt.subplots(4, 1, figsize=(12, 16), sharex=True)
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
    ax[3].set_ylabel("m/s²")
    ax[3].legend()

    ax[3].xaxis.set_major_formatter(mpl_dates.DateFormatter('%H:%M:%S'))

    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.tight_layout()
    plt.savefig("main_graphs.png")
    plt.close(fig)

    print("Main graphs have been saved to main_graphs.png.")


def plot_stage_bar_charts(distances, completed_stages, next_stages):
    completed_stage_counts = {stage: completed_stages.count(
        stage) for stage in set(completed_stages)}
    next_stage_counts = {stage: next_stages.count(
        stage) for stage in set(next_stages)}

    fig, ax = plt.subplots(1, 2, figsize=(12, 6))
    ax[0].bar(completed_stage_counts.keys(), completed_stage_counts.values())
    ax[0].set_title("Completed Stages")
    ax[0].set_ylabel("Counts")

    ax[1].bar(next_stage_counts.keys(), next_stage_counts.values())
    ax[1].set_title("Next Stages")
    ax[1].set_ylabel("Counts")

    plt.tight_layout()
    plt.savefig("stage_bar_charts.png")
    plt.close(fig)

    print("Stage bar charts saved to stage_bar_charts.png")


def plot_correlation_matrix(light_levels, distances, temperatures, acc_x, acc_y, acc_z):
    data = np.array(
        [light_levels, distances, temperatures, acc_x, acc_y, acc_z]).T
    corr_matrix = np.corrcoef(data, rowvar=False)
    sns.heatmap(corr_matrix, annot=True, cmap="coolwarm", xticklabels=[
                "Light", "Distance", "Temperature", "Acc X", "Acc Y", "Acc Z"], yticklabels=["Light", "Distance", "Temperature", "Acc X", "Acc Y", "Acc Z"])
    plt.title("Correlation Matrix")
    plt.savefig("correlation_matrix.png")
    plt.close()

    print("Correlation matrix saved to correlation_matrix.png")


if __name__ == "__main__":
    main()
