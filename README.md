# Memory-management
Assignment - SPL course (BSC in computer science)

# Reconstruction Simulation

## Overview
This project is a **C++ simulation** for evaluating different **reconstruction plans** in the fictional land of SPLand, which has been affected by war. The system manages **settlements, facilities, and reconstruction plans** using object-oriented programming principles.

## Features
- **Object-Oriented Design**: Uses classes and the **Rule of 5** for memory management.
- **Configurable Simulation**: Reads an input configuration file to initialize the environment.
- **Interactive Actions**: Supports user commands for adding settlements, facilities, and managing reconstruction plans.
- **Selection Policies**: Implements multiple strategies for facility selection:
  - Naïve (round-robin)
  - Balanced (minimizing score difference)
  - Economy-focused
  - Sustainability-focused
- **Logging & State Management**: Tracks actions and allows **backup/restore** of the simulation state.

## How to Run
1. **Compile the project** (ensure you have a C++11+ compiler):
   ```sh
   make
