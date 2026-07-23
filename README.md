# Dynamic Memory Allocation Simulator

A C++ console-based simulator that demonstrates the core concepts of **dynamic memory allocation** and **process-level memory management**.  
The project provides an interactive environment for creating processes, allocating memory blocks, resizing allocated memory, protecting memory regions, and monitoring allocation history.

---

## Overview

This project simulates how an operating system manages dynamically allocated memory for multiple processes.

Each process can own multiple memory blocks, and every block supports common memory management operations such as allocation, deallocation, reallocation, reading, and writing.

The simulator also records every memory operation into a history log, allowing users to review memory activity throughout execution.

---

## Features

- Process-based memory management
- Dynamic memory allocation (`malloc`)
- Dynamic memory deallocation (`free`)
- Dynamic memory reallocation (`realloc`)
- Support for multiple data types
- Read and write operations on allocated memory
- Read-only protection for blocks and individual indexes
- Operation history logging
- Interactive command-line interface

---

## Supported Data Types

| Type | Description |
|------|-------------|
| `char` | Character values |
| `int` | Integer values |
| `float` | Floating-point values |
| `string` | Dynamically allocated strings |
| `mix` | Union type supporting multiple primitive types |

---

## Supported Commands

| Command | Description |
|---------|-------------|
| `allocate` | Create and allocate a new memory block |
| `deallocate` | Free an allocated block |
| `reallocate` | Resize an existing memory block |
| `write` | Store data in a memory block |
| `read` | Read stored data |
| `lam` | List all allocated memory blocks |
| `history` | Display the operation history |
| `clear` | Clear the terminal |
| `exit` | Exit the simulator |

---

## Project Structure

```
Process
│
├── Block 1
│      ├── Type
│      ├── Size
│      ├── Data
│      └── Protection Status
│
├── Block 2
│
└── ...
```

Each process maintains its own collection of memory blocks, allowing independent memory management.

---

## Memory Operations

The simulator supports the complete lifecycle of a memory block:

```
Allocate
      │
      ▼
Write Data
      │
      ▼
Read Data
      │
      ▼
Reallocate (optional)
      │
      ▼
Free Memory
```

All performed operations are recorded in `history.txt`.

---

## Build

Compile using any C++ compiler.

### Linux / macOS

```bash
g++ main.cpp -o simulator
./simulator
```

### Windows (MinGW)

```bash
g++ main.cpp -o simulator.exe
simulator.exe
```

---

## Concepts Demonstrated

This project demonstrates several important concepts related to operating systems and memory management, including:

- Dynamic memory allocation
- Heap memory management
- Memory ownership
- Process abstraction
- Runtime memory resizing
- Access protection
- Operation logging

---

## Future Improvements

Possible future extensions include:

- First Fit allocation strategy
- Best Fit allocation strategy
- Worst Fit allocation strategy
- Memory fragmentation analysis
- Memory usage statistics
- Graphical user interface
- File persistence
- Smart pointer implementation
- STL container replacement for custom linked list

---

## Technologies

- C++
- Standard Template Library (STL)
- Dynamic Memory Management
- Object-Oriented Programming

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for more information.

---

## Author

**Amir.Mp**

Computer Science Student

Interested in:

- Artificial Intelligence
- Operating Systems
- Algorithms
- Backend Development
- System Programming
