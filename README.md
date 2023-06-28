# NFGen

## Description
BUAA 2023 Spring Cryptography.

## How to run it?

This project is build with MSVC in Visual Studio. All working directories are set. You should manually create a folder called `Work\` parallel to `src\`, and copy `src\config.yaml` to `Work\config.yaml`. All global variables are set in this yaml file.

### Step 1. Configure target function

Modify `main.cpp` to set the target function and interval for NFGen. The default function is GELU activation function in [-10.0, 3.0].

### Step 2. Run NFGen algorithm

Build the project `NFG` and run it. You should see some output in the console, and get `sample.csv` for standard function and `function.csv` for the NFGen generated function. And `result.txt` for the piecewise polynomial.

### Step 3. Draw plot

You can ignore the `Visualizer` project, and treat `Visualizer\Visualizer.py` as a regular Python script. Install and mange the packages the way you like, and run it under directory `Visualizer\`. It will automatically read the result of NFGen algorithm, and draw the plot. 😀

