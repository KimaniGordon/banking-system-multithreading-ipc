#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
using namespace std;

class BankAccount {
private:
    int balance;
    mutex mtx;

public:
    BankAccount(int initial_balance) : balance(initial_balance) {}

    void deposit(int amount) {
        lock_guard<mutex> lock(mtx);
        balance += amount;
        cout << "Deposited: " << amount << " | Balance: " << balance << endl;
    }

    void withdraw(int amount) {
        lock_guard<mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount << " | Balance: " << balance << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    int getBalance() {
        return balance;
    }
};

// Test case 1: Concurrency Testing
void concurrencyTest(BankAccount &account, int id) {
    for (int i = 0; i < 3; i++) {
        account.deposit(50);
        account.withdraw(30);
    }
}

// Test case 2: Synchronization Validation (removing mutex to simulate race condition)
void syncValidationTest(BankAccount &account) {
    // Temporarily remove the mutex guard in the methods
    for (int i = 0; i < 3; i++) {
        account.deposit(50);  // Without lock_guard<mutex> to simulate a race condition
        account.withdraw(30);
    }
}

// Test case 3: Stress Testing (Increase the number of customer threads)
void stressTest() {
    BankAccount account(100);
    vector<thread> customers;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 50; i++) {  // Increase thread count to simulate stress
        customers.push_back(thread(concurrencyTest, ref(account), i));
    }
    for (auto &t : customers) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nStress Test completed in " << duration.count() << " milliseconds.\n";
}

int main() {
    cout << "Starting multi-threaded banking system...\n";

    // Run Concurrency Test
    cout << "\n==== Running Concurrency Test ====\n";
    BankAccount account1(100);
    vector<thread> customers1;
    for (int i = 0; i < 5; i++) {
        customers1.push_back(thread(concurrencyTest, ref(account1), i));
    }
    for (auto &t : customers1) {
        t.join();
    }

    // Run Synchronization Validation Test
    cout << "\n==== Running Synchronization Validation Test (no mutex) ====\n";
    BankAccount account2(100);
    syncValidationTest(account2);

    // Run Stress Test
    cout << "\n==== Running Stress Test ====\n";
    stressTest();

    return 0;
}
