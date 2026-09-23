#include <vector>  // for arrays
#include <cstdint>  // uint64_t
#include <random>  // mt19937_64
#include <iostream>  // cout

#include "timer.h"  // Importing timer written in lab prep

constexpr size_t SIZE = 16000000;
constexpr int AXIS_LENGTH = 4000;

int main() {
    // Initialize first array
    std::vector<uint64_t> arrayRowMajor = std::vector<uint64_t>();
    arrayRowMajor.reserve(SIZE);

    // Initialize second array
    std::vector<uint64_t> arrayColMajor = std::vector<uint64_t>();
    arrayColMajor.reserve(SIZE);

    std::mt19937_64 rng(0);  // Seeded to 0 as of initialization

    // Fill arrayRowMajor in rowMajorOrder
    for (int row = 0; row < AXIS_LENGTH; row++) {
        for (int col = 0; col < AXIS_LENGTH; col++) {
            // Calculate index as (row * number_of_cols) + col
            arrayRowMajor[(row * AXIS_LENGTH) + col] = rng();
        }
    }

    // Reseed rng
    rng.seed(0);

    // Fill arrayColMajor in colMajorOrder
    for (int col = 0; col < AXIS_LENGTH; col++) {
        for (int row = 0; row < AXIS_LENGTH; row++) {
            // Calculate index as (col * number_of_rows) + row
            arrayColMajor[(col * AXIS_LENGTH) + row] = rng();
        }
    }

    // Initialize a timer, and sums
    Timer timer = Timer();
    uint64_t rowMajorSum = 0;
    uint64_t colMajorSum = 0;

    // Reset timer, then sum in row major order as in the nested loop used to initialize arrayRowMajor
    timer.restart();
    for (int row = 0; row < AXIS_LENGTH; row++) {
        for (int col = 0; col < AXIS_LENGTH; col++) {
            // Calculate index as (row * number_of_cols) + col
            rowMajorSum += arrayRowMajor[(row * AXIS_LENGTH) + col];
        }
    }
    uint64_t rowMajorTime = timer.click<Timer::Micros>();

    // Reset timer, then sum in column major order as in the nested loop used to initialize arrayColMajor
    timer.restart();
    for (int col = 0; col < AXIS_LENGTH; col++) {
        for (int row = 0; row < AXIS_LENGTH; row++) {
            // Calculate index as (col * number_of_rows) + row
            colMajorSum += arrayColMajor[(col * AXIS_LENGTH) + row];
        }
    }
    uint64_t colMajorTime = timer.click<Timer::Micros>();

    // Print times and sums
    std::cout << rowMajorTime << " " << rowMajorSum << "\n";
    std::cout << colMajorTime << " " << colMajorSum << "\n";

    return 0; 
}