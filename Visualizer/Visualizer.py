from tkinter import font
from matplotlib import pyplot as plt
import csv
import tensorflow as tf


if __name__ == "__main__":
    # Get NFGen data.
    filename = "../Work/function.csv"
    with open(filename) as f:
        reader = csv.reader(f)
        header_row = next(reader)
        x_set = []
        y_set = []
        for row in reader:
            x_set.append(float(row[0]))
            y_set.append(float(row[1]))

    # Get standard data.
    std_y_set = []
    for x in x_set:
        std_y_set.append(tf.nn.gelu(x))

    # Draw plot.
    fig = plt.figure(dpi=128, figsize=(10, 6))
    plt.plot(x_set, std_y_set, c="red")
    plt.plot(x_set, y_set, c="blue")

    plt.title("NFGen result on GELU activation function", fontsize=20)
    plt.xlabel("x", fontsize=16)
    plt.ylabel("y", fontsize=16)
    plt.tick_params(axis="both", which="major", labelsize=16)
    
    plt.show()
