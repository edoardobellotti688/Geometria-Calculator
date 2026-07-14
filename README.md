# Geometria - Matrix & Vector Calculator

A scientific calculator for matrices and vectors written in C, featuring an advanced eigenvalue calculation engine and automatic state saving.

## 🛠️ How to Compile and Run

The project includes a `Makefile` configured for compilation on both Windows and Linux.

### Prerequisites
Ensure you have a C compiler (`gcc`) and the `make` utility installed on your system.

### Compilation
Open your terminal in the project directory and run:
```bash
make

Execution
    Linux:  
        ./geometria

    Windows:
        geometria.exe

🛡️ 1. Security Notices (Windows SmartScreen / Norton Antivirus)
Since this is an independently developed application and is not digitally signed with a commercial code-signing certificate, the executable file (.exe) may be flagged as "unknown" by Windows SmartScreen or by strict antivirus software such as Norton the first time it is launched.  
The program is completely safe to use. If it is blocked, follow these steps:  
    Windows SmartScreen: 
        On the blue warning screen, click "More info" and then "Run anyway". This warning is only displayed the first time the program is executed.  
    Norton / Other Antivirus Software: 
        If the executable is moved to quarantine, open your antivirus notification center, locate the file, and choose "Restore", "Allow", or "Add to Exclusions" (the wording may vary depending on the antivirus).

💾 2. Data File Management (data.bin)
The first time you launch the program, it is completely normal to receive a warning indicating that the data file could not be found.  You do not need to create an empty file manually. The program handles this situation automatically:  
When you exit the program (by entering -1 in the main menu), the correct data.bin file will be created automatically in the program folder. From the next launch onward, the file will be detected automatically, and the saved history of matrices and vectors will be loaded without displaying any warning.  
    🔄 How to reset your data:  
        Close the program.  
        Delete the data.bin file from the program folder.  
        Launch the program again. 
        A new, clean data file will be generated automatically the next time the program is closed. 

📐 3. Recommended Matrix Size
Although the program supports matrices of various dimensions, the highly recommended maximum size for a matrix is 99 x 99. Larger matrices may significantly reduce readability and usability within the console interface.

📊 4. Binary File Format Specification
If you wish to import a binary file generated externally, ensure that it strictly follows the sequential structure described below.  
    1. First Value  
        A 4-byte integer (int) representing the total number of objects stored in the file.  
    2. Subsequent Data
        Each object must be written sequentially and consist of the following fields:  
            Object name: 4 bytes (ASCII string, maximum of 3 characters plus the null terminator).  Number of rows: 4-byte integer.  
            Number of columns: 4-byte integer.  
            Matrix elements: A sequential list of floating-point values (4-byte float each). The total number of values must be equal to rows * columns.

🧮 5. Eigenvalue Calculations (QR Algorithm & Limits)  
The program computes eigenvalues using an advanced Shifted QR Decomposition Algorithm with Deflation (utilizing the Wilkinson shift). This approach allows the engine to resolve both real and complex eigenvalues.  
How it works:
    Shifting: 
        The algorithm dynamically shifts the matrix eigenvalues at each step. This breaks mathematical stalls and dramatically accelerates convergence.  
    Deflation: 
        Once an eigenvalue (or a 2x2 complex block) is isolated, it is extracted algebraically, and the working matrix is shrunk ("deflated"). This saves CPU cycles and ensures high precision.  
    Mathematical Limitations:
        3+ Equal-Modulus Clusters: Because the algorithm uses real shifts, it can easily resolve pairs of equal absolute value (e.g., +x/-x, or a single complex conjugate pair). However, it cannot mathematically resolve larger groups of 3 or more eigenvalues that share the exact same modulus (like the complex roots of unity) and refuse to isolate.
        Defective Matrices: Highly ill-conditioned or defective matrices may struggle to converge within the limit of 1000 iterations due to floating-point precision limits.
        "Graceful Degradation": If convergence fails, the program does not crash; it displays a warning, prints the eigenvalues resolved up to that point, and outputs the unresolved block of the matrix.
