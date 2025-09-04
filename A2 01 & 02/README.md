# Two-Pass Macro Processor (C++)

This project implements a **two-pass macro processor** in C++.  
It takes an assembly program with macros, processes it in **two passes**, and produces fully expanded code.

---

## 📂 Project Structure

```
A2 01 & 02/
│
├── inputs/
│ └── input.asm # Assembly program with macro definitions
│
├── outputs/
│ ├── IC.txt # Intermediate Code (macro calls, no definitions)
│ ├── MDT.txt # Macro Definition Table
│ ├── MNT.txt # Macro Name Table
│ └── output.txt # Final expanded source after Pass-II
│
└── 2-pass-macro.cpp # Combined Pass-I and Pass-II implementation
```

---

## ⚙️ Compilation & Execution

From inside the `A2 01 & 02` folder:

```bash
g++ -std=c++17 -O2 2-pass-macro.cpp -o macro_processor
./macro_processor
```
- Input file: `inputs/input.asm`
- Pass-I output files: `outputs/MNT.txt`, `outputs/MDT.txt`, `outputs/IC.txt`
- Pass-II output file: `outputs/output.txt`

### ✅ Features

- Implements both Pass-I (MNT/MDT/IC generation) and Pass-II (macro expansion).
- Supports multiple macros and positional parameters (&ARG).
- Generates clean intermediate and final outputs in the outputs/ folder.
---

### 📌 Usage Notes
- Edit `inputs/input.asm` to test with your own macro definitions.
- Run the processor to regenerate all files in `outputs/`.
- Check `output.txt` for the final expanded code.
