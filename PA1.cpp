#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>

// Function to find primes in a given range for a specific thread
void findPrimesInRange(int start, int end, std::vector<bool>& primes) {
    for (int i = 2; i <= std::sqrt(end); ++i) {
        if (primes[i]) {
            for (int j = i * i; j <= end; j += i) {
                primes[j] = false;
            }
        }
    }
}

int main() {
    const int N = 100000000; // 10^8
    const int numThreads = 8;

    // Create a boolean vector to store prime information
    std::vector<bool> primes(N + 1, true);

    // Start the timer
    auto startTime = std::chrono::high_resolution_clock::now();

    // Split the range among threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        int start = (i * N) / numThreads + 2; // Avoid checking 0 and 1
        int end = ((i + 1) * N) / numThreads;
        threads.emplace_back(findPrimesInRange, start, end, std::ref(primes));
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Stop the timer
    auto endTime = std::chrono::high_resolution_clock::now();
    auto executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Count the primes and calculate the sum
    int totalCount = 0;
    long long sum = 0;
    for (int i = 2; i <= N; ++i) {
        if (primes[i]) {
            totalCount++;
            sum += i;
        }
    }

    // Print the results to primes.txt
    std::ofstream outputFile("primes.txt");
    outputFile << executionTime << " " << totalCount << " " << sum << std::endl;

    // Print the top ten maximum primes
    for (int i = N; totalCount > 0 && i >= 2; --i) {
        if (primes[i]) {
            outputFile << i << " ";
            totalCount--;
        }
    }

    return 0;
}
