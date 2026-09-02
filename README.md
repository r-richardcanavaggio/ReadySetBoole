# ReadySetBoole

[![C++14](https://img.shields.io/badge/C%2B%2B-14-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Build System](https://img.shields.io/badge/Build-GNU%20Make-6BA81E?logo=gnu)](Makefile)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20STL)-brightgreen)](#academic-context)
[![Domain](https://img.shields.io/badge/Domain-Boolean%20Algebra%20%7C%20AST%20%7C%20SAT-blueviolet)](#architecture--modules)

> A comprehensive C++14 implementation exploring boolean algebra, hardware-level bitwise arithmetic, symbolic AST manipulation (NNF/CNF transformations), SAT solving, and Morton space-filling curves.

---

## 📌 Academic Context

* **Institution**: École 42 (Post-Common Core / Advanced Curriculum)
* **Type**: Individual project
* **Language & Standard**: Modern C++ (C++14), standard library only
* **Core Constraints**: Zero external dependencies (all parsers, AST engines, and solvers built strictly from scratch).

---

## 🏗️ Architecture & Modules

The project is structured into **12 progressive standalone modules** (`ex00` to `ex11`), categorized into 4 core computer science pillars:

```text
ReadySetBoole/
├── Low-Level Arithmetic & Bits   ──> ex00 (Adder), ex01 (Multiplier), ex02 (Gray Code)
├── Symbolic Logic & AST Engines  ──> ex03 (RPN Eval), ex04 (Truth Table), ex05 (NNF), ex06 (CNF)
├── Solvers & Set Theory          ──> ex07 (SAT Solver), ex08 (Powerset), ex09 (Set Evaluation)
└── Spatial Indexing & Curves     ──> ex10 (Morton Curve 2D->1D), ex11 (Inverse Morton 1D->2D)
```

| Pillar | Module | Topic | Description |
| :--- | :---: | :--- | :--- |
| **1. Low-Level ALU & Bits** | `ex00` | **Bitwise Adder** | Adds two 32-bit integers using only bitwise gates (`^`, `&`, `<<`) without `+`. |
| | `ex01` | **Bitwise Multiplier** | Multiplies integers using shift-and-add ALU logic without `*`. |
| | `ex02` | **Gray Code** | Computes Reflected Binary Gray Code for state-transition safety. |
| **2. Propositional Logic & AST** | `ex03` | **RPN Boolean Evaluator** | Stack-based evaluation of boolean formulas (`!`, `&`, `\|`, `^`, `>`, `=`). |
| | `ex04` | **Truth Table Generator** | Parses formulas with variables (`A`-`Z`) and prints truth tables. |
| | `ex05` | **NNF Transformer** | Builds an AST to transform formulas into Negation Normal Form (De Morgan). |
| | `ex06` | **CNF Transformer** | Rewrites AST into Conjunctive Normal Form via boolean distributivity. |
| **3. Solvers & Set Theory** | `ex07` | **SAT Solver** | Solves the Boolean Satisfiability problem (checks if formula is satisfiable). |
| | `ex08` | **Powerset Generator** | Computes the power set $\mathcal{P}(S)$ of a discrete integer set. |
| | `ex09` | **Set Logic Evaluator** | Evaluates propositional logic expressions applied directly to sets. |
| **4. Space-Filling Curves** | `ex10` | **Morton Curve (Z-Order)** | Interleaves 16-bit $(X, Y)$ coordinates into a normalized 1D scalar $[0, 1)$. |
| | `ex11` | **Inverse Morton Curve** | Reconstructs original 2D coordinates $(X, Y)$ from a 1D Morton scalar. |

---

## 🧠 Key Engineering Highlights & Learnings

### 1. Hardware-Level Arithmetic Emulation (ALU Logic)
* **Half & Full Adders from Scratch**: Implemented addition using XOR (`^`) for sum bits and AND (`&`) shifted left (`<< 1`) for carry propagation until carry becomes zero.
* **Russian Peasant / Shift-and-Add**: Built integer multiplication purely on top of the custom bitwise adder and bit shifts.

### 2. Abstract Syntax Tree (AST) & Symbolic Rewriting
* **Parsing RPN into Trees**: Built a dynamic binary tree structure representing logical operations and variables.
* **Logical Normalization Pipeline**:
  * **Material Implication & Equivalence Elimination**: $A \Rightarrow B \equiv \neg A \lor B$ and $A \Leftrightarrow B \equiv (A \land B) \lor (\neg A \land \neg B)$.
  * **De Morgan's Laws & Double Negation**: Pushed negations down to literals for **Negation Normal Form (NNF)**.
  * **Conjunctive Normal Form (CNF)**: Recursively distributed $\lor$ over $\land$ ($A \lor (B \land C) \equiv (A \lor B) \land (A \lor C)$) with tree cloning and canonical node deduplication.

### 3. Solvers & Combinatorial Algorithms
* **Boolean Satisfiability (SAT)**: Formulated propositional satisfiability testing by traversing the variable assignment space ($2^N$ states) and verifying expression models.
* **Combinatorics**: Generated discrete subsets using bitmask enumeration techniques.

### 4. Spatial Indexing & Bit Interleaving
* **Z-Order Curve (Morton Code)**: Mapped 2D discrete space into 1D continuous space using bit dilation and interleaving, a fundamental technique in spatial databases, quadtrees, and GPU texture caches.

---

## 🚀 Getting Started

### Prerequisites
* A C++ compiler supporting **C++14** (`clang++` or `g++`)
* `make`

### Compilation

```bash
# Build all 12 modules into bin/
make

# Or compile an individual exercise
make ex05
make ex06
```

### Cleaning Artifacts

```bash
make clean   # Remove object files (.obj)
make fclean  # Remove object files and compiled binaries (bin/)
make re      # Clean rebuild
```

---

## 💻 Showcase & Usage Examples

### 1. Truth Table Generation (`ex04`)
Evaluates any RPN formula with variable inputs:
```bash
$ ./bin/ex04
Enter propositional RPN formula: AB=
| A | B | = |
|---|---|---|
| 0 | 0 | 1 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |
```

### 2. AST Visualizer & NNF Transformation (`ex05`)
Parses `AB=` (equivalence $A \Leftrightarrow B$), displays the input AST, converts to NNF, and displays the transformed AST:
```bash
$ ./bin/ex05 "AB="
┌── B
=
└── A
----------
    ┌── !
    │   └── B
┌── &
│   └── !
│       └── A
|
│   ┌── B
└── &
    └── A
AB&A!B!&|
```

### 3. CNF Normalization (`ex06`)
Transforms $(A \lor B) \land C$ directly into Conjunctive Normal Form:
```bash
$ ./bin/ex06 "AB|C&"
┌── C
&
│   ┌── B
└── |
    └── A
----------
┌── C
&
│   ┌── B
└── |
    └── A
AB|C&
```

### 4. SAT Solver (`ex07`)
Determines if a propositional formula is satisfiable:
```bash
$ ./bin/ex07 "AB|!"
true

$ ./bin/ex07 "AA!&"
false
```

### 5. Morton Curve (2D $\leftrightarrow$ 1D Mapping) (`ex10` & `ex11`)
```bash
# Map 2D coordinate (12345, 54321) to normalized scalar
$ ./bin/ex10 12345 54321
0.652833

# Inverse mapping from normalized scalar to 2D coordinates
$ ./bin/ex11 0.652833
12345 54321
```
