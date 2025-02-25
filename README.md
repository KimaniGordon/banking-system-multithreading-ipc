# Banking System: Multi-threading & IPC

## Project Overview

This repository contains two projects related to multi-threading and inter-process communication (IPC) in C++:

1. **Project 1A: Multi-threading Banking System**
   - This project demonstrates how to implement a multi-threaded banking system where multiple customers can deposit and withdraw from a shared bank account concurrently.
   - Synchronization is achieved using `mutex` to prevent race conditions.

2. **Project 1B: IPC with Pipes**
   - This project uses inter-process communication (IPC) through pipes to log transaction information between a parent process and a child process.
   - The project demonstrates handling data integrity, broken pipe errors, and performance benchmarking for large data transmission.

## Compilation & Execution Instructions

### Project 1A: Multi-threading Banking System
1. Compile the multi-threading banking system:
   ```bash
   g++ -std=c++14 -pthread Project1A.cpp -o Project1A

