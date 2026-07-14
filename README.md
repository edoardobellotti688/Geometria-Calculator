# Geometria - Matrix & Vector Calculator

A scientific calculator for matrices and vectors written in **C**, featuring an advanced eigenvalue calculation engine and automatic state saving.

---

# 🛠️ How to Compile and Run

The project includes a `Makefile` configured for compilation on both **Windows** and **Linux**.

## Prerequisites

Ensure you have:

- A C compiler (`gcc`)
- The `make` utility

installed on your system.

## Compilation

Open a terminal inside the project directory and run:

```bash
make
```

## Execution

### Linux

```bash
./geometria
```

### Windows

```bash
geometria.exe
```

---

# 🛡️ 1. Security Notices (Windows SmartScreen / Antivirus)

Since this is an independently developed application and is **not digitally signed** with a commercial code-signing certificate, the executable (`.exe`) may be flagged as **unknown** by Windows SmartScreen or by strict antivirus software (such as Norton) the first time it is launched.

The program is completely safe to use.

If it is blocked:

### Windows SmartScreen

On the blue warning screen:

1. Click **More info**.
2. Click **Run anyway**.

This warning normally appears only the first time the application is executed.

### Norton / Other Antivirus Software

If the executable is moved to quarantine:

1. Open your antivirus notification center.
2. Locate the executable.
3. Choose **Restore**, **Allow**, or **Add to Exclusions** (wording varies depending on the antivirus).

---

# 💾 2. Data File Management (`data.bin`)

The first time you launch the program, it is completely normal to receive a warning indicating that the data file could not be found.

You **do not** need to create an empty file manually.

The program handles everything automatically.

When you exit the application (by entering `-1` in the main menu), the correct `data.bin` file is automatically created inside the program folder.

On subsequent launches, the file is loaded automatically and all previously saved matrices and vectors become available.

## Resetting your data

1. Close the program.
2. Delete `data.bin`.
3. Launch the program again.

A new empty database will be created automatically the next time the program exits.

---

# 📐 3. Recommended Matrix Size

Although the program supports matrices of many different dimensions, the recommended maximum size is:

**99 × 99**

Larger matrices are supported but may significantly reduce readability and usability within the console interface.

---

# 📊 4. Binary File Format Specification

If you want to import a binary file generated externally, it **must** follow the exact structure below.

## File Layout

### Header

A 4-byte integer (`int`) representing the total number of stored objects.

### For each object

| Field | Type |
|-------|------|
| Object name | 4-byte ASCII string (maximum 3 characters + `\0`) |
| Rows | 4-byte integer |
| Columns | 4-byte integer |
| Matrix elements | Sequential 4-byte `float` values |

The number of stored floating-point values must equal:

```
rows × columns
```

---

# 🧮 5. Eigenvalue Calculations

The program computes eigenvalues using an advanced **Shifted QR Decomposition Algorithm with Deflation**, employing the **Wilkinson Shift**.

The implementation supports both **real** and **complex** eigenvalues.

## Algorithm Overview

### Shifting

The QR iteration dynamically applies Wilkinson shifts at every iteration.

This dramatically accelerates convergence while avoiding many numerical stagnation cases.

### Deflation

Whenever an eigenvalue (or a 2×2 complex block) becomes isolated, it is extracted algebraically and removed from the active matrix.

This reduces computational cost while improving numerical stability.

---

# Mathematical Limitations

Like every numerical eigenvalue algorithm, the QR method has some inherent mathematical limitations.

### Equal-Modulus Clusters

Because the implementation uses **real Wilkinson shifts**, it can reliably resolve:

- real eigenvalue pairs such as ±x
- complex conjugate pairs

However, it may fail to separate larger groups (three or more) of eigenvalues sharing exactly the same modulus (for example, certain roots of unity).

### Defective or Ill-Conditioned Matrices

Matrices that are defective or highly ill-conditioned may not converge within the current iteration limit (1000 iterations) due to floating-point precision limitations.

### Graceful Degradation

If convergence is not achieved:

- the program **does not crash**;
- all eigenvalues successfully computed up to that point are displayed;
- the remaining unresolved matrix block is printed for inspection.

---

# 📄 License

This project is released under the **MIT License**.

See the `LICENSE` file for details.
