#include <cassert>  // assert
#include <cstddef>  // size_t
#include <cstdint>  // uint64_t
#include <random> // mt19937_64
#include <iostream>  // cout
#include <vector>  //vector
#include <cassert>  // assert

#include "timer.h"
#include "p2a.h"

constexpr size_t SIZE = 16777216;
constexpr size_t SIZE_CONV = 262144;

size_t rowMajorIndexA(size_t x, size_t y, size_t z) {
    // Gets x, y, and z and returns index to that point
    // index = x + (y * x_length) + (z * x_length * y_length)
    return (x + (y * 256) + (z * 256 * 256));
}
size_t rowMajorIndexConv(size_t x, size_t y, size_t z) {
    return (x + (y * 64) + (z * 64 * 64));
}

int main() { 
    std::vector<uint64_t> rowMajorArray = std::vector<uint64_t>(SIZE);  // 1D array of size 16777216 to represent 3D array of 256*256*256
    std::vector<uint64_t> mortonArray = std::vector<uint64_t>(SIZE);

    std::mt19937_64 rng(0); // Seeded to zero

    // Filling rowMajorArray in row major order is just filling in from 0 to 16777216 
    // I drew a picture to confirm this for myself
    for (uint64_t i = 0; i < SIZE; i++){
        rowMajorArray[i] = rng();
    }

    // Filling mortonArray
    int index = 0;
    for (int z = 0; z < 256; z++) {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                uint64_t index = morton3d(x, y, z);
                mortonArray[index] = rowMajorArray[index];  // to copy rowMajorArray into mortonArray in order
                ++index;
            }
        }
    }

    // Creating my kernels
    std::vector<uint64_t> kernelRowMajor = std::vector<uint64_t>(64);  // 64 is the total number of entries for 4x4x4
    std::vector<uint64_t> kernelMorton = std::vector<uint64_t>(64);

    // Fill kernels with appropriate values
    index = 0;
    for (int z = 0; z < 4; z++) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                kernelRowMajor[index] = (x + y + z);
                kernelMorton[morton3d(x, y, z)] = (x + y + z);
                 ++index;
            }
        }
    }

    // Shape after convolution is (64, 64, 64) based on the given formula, padding = 0, stride = 4
    // 64*64*64 = 262144
    std::vector<uint64_t> convolvedRowMajor = std::vector<uint64_t>(SIZE_CONV);
    std::vector<uint64_t> convolvedMorton = std::vector<uint64_t>(SIZE_CONV);
    
    // Instantiate timer
    Timer timer = Timer();

    // Convolve row major array

    timer.restart();
    for (int arr_z = 0; arr_z < 64; arr_z++) {
        for (int arr_y = 0; arr_y < 64; arr_y++) {
            for (int arr_x = 0; arr_x < 64; arr_x++) {
                // for each box:
                int kernelIndex = 0;
                uint64_t boxSum = 0;
                for (int z = 0; z < 4; z++) {
                    for (int y = 0; y < 4; y++) {
                        for (int x = 0; x < 4; x++) {
                            boxSum += (kernelRowMajor[kernelIndex])*(rowMajorArray[rowMajorIndexA(arr_x*4 + x, arr_y*4 + y, arr_z*4 + z)]);
                            kernelIndex++;
                        }
                    }
                }
                // Once dot product of box calculated, add to conv array
                convolvedRowMajor[rowMajorIndexConv(arr_x, arr_y, arr_z)] = boxSum;
            }
        }
    }
    uint64_t rowMajorTime = timer.click<Timer::Micros>();

    // Convolve morton array
    uint64_t blockSum = 0;

    timer.restart();
    for (uint64_t i = 0; i < SIZE_CONV; i++) {
        // just iterate over each value of array
        blockSum = 0;
        for (uint64_t j = 0; j < 64; j++) {
        // Iterates through each value of the kernel
        blockSum += kernelMorton[j] * mortonArray[i*64 + j];
        }
        convolvedMorton[i] = blockSum;
    }
    uint64_t mortonTime = timer.click<Timer::Micros>();

    std::cout << rowMajorTime << "\n";
    std::cout << mortonTime << "\n";

    // Verify results
    for (uint64_t z = 0; z < 64; z++) {
        for (uint64_t y = 0; y < 64; y++) {
            for (uint64_t x = 0; x < 64; x++) {
                assert(convolvedRowMajor[rowMajorIndexConv(x, y, z)] == convolvedMorton[morton3d(x, y, z)]);
            }
        }
    }

    return 0; 
}