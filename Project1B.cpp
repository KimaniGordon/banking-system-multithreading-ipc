#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <chrono>
using namespace std;

// Data Integrity Test
void dataIntegrityTest(int fd[2]) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed" << endl;
    } else if (pid == 0) {  // Child process (Writer)
        close(fd[0]);
        string message = "Transaction Log: Deposit 50, Withdraw 30";
        write(fd[1], message.c_str(), message.size() + 1);
        close(fd[1]);
    } else {  // Parent process (Reader)
        close(fd[1]);
        char buffer[100] = {0};
        read(fd[0], buffer, sizeof(buffer) - 1);
        cout << "Bank Log Process received: " << buffer << endl;
        close(fd[0]);
        wait(NULL);
    }
}

// Broken Pipe Handling Test
void brokenPipeTest(int fd[2]) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed" << endl;
    } else if (pid == 0) {  // Child process (Writer)
        close(fd[0]);
        close(fd[1]);  // Close the write-end before writing to simulate a broken pipe
        string message = "Transaction Log: Deposit 50, Withdraw 30";
        if (write(fd[1], message.c_str(), message.size()) == -1) {
            cerr << "Pipe error detected" << endl;
        }
        // No need to close fd[1] as it was already closed
    } else {  // Parent process (Reader)
        close(fd[1]);
        char buffer[100] = {0};
        read(fd[0], buffer, sizeof(buffer) - 1);
        cout << "Bank Log Process received: " << buffer << endl;
        close(fd[0]);
        wait(NULL);
    }
}

// Performance Benchmarking Test (1MB data)
void performanceBenchmarkTest(int fd[2]) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed" << endl;
    } else if (pid == 0) {  // Child process (Writer)
        close(fd[0]);
        string message(1000000, 'X');  // 1MB of data
        auto start = chrono::high_resolution_clock::now();
        write(fd[1], message.c_str(), message.size());
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Data transmission took " << duration.count() << " milliseconds.\n";
        close(fd[1]);
    } else {  // Parent process (Reader)
        close(fd[1]);
        char buffer[1000000] = {0};  // Buffer to receive 1MB
        read(fd[0], buffer, sizeof(buffer) - 1);
        cout << "Received " << strlen(buffer) << " bytes of data.\n";
        close(fd[0]);
        wait(NULL);
    }
}

int main() {
    cout << "Starting IPC transaction logging...\n";

    // Test Data Integrity
    int fd1[2];
    if (pipe(fd1) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }
    cout << "\nRunning Data Integrity Test...\n";
    dataIntegrityTest(fd1);

    // Test Broken Pipe Handling
    int fd2[2];
    if (pipe(fd2) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }
    cout << "\nRunning Broken Pipe Handling Test...\n";
    brokenPipeTest(fd2);

    // Test Performance Benchmarking
    int fd3[2];
    if (pipe(fd3) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }
    cout << "\nRunning Performance Benchmarking Test...\n";
    performanceBenchmarkTest(fd3);

    return 0;
}


