#include <cassert>  // assert
#include <cstddef>  // size_t
#include <cstdint>  // uint64_t, uint32_t
#include <random> // mt19937_64
#include <iostream>  // cout
#include <bitset>  //bitset

// References: https://www.geeksforgeeks.org/cpp/cpp-bitwise-operators/

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
    // d = 3, k = 64 -> expand each one by 3
    uint64_t newNum = 0b0;

    // Could probably be spacially optimized to use one value for nextDigit and one value for placedBit but I don't have time right now
    // Basically replicating expand, but expanding all three inputs instead of just one
    for (uint64_t place = 0; place < 64; place++) {
        uint64_t nextPlace = place*3;  // dimension is 3, so we are scaling by 3
        if (nextPlace >= 64) { break; }  // If next place >= 64, it those bits would be truncated anyways - no reason to calculate

        // get next digit isolated -> 0b0...00 or 0b0...01 for each x, y, and z
        uint64_t nextDigitx = x >> place;  // shifts x so that the digit in question is at place 0
        uint64_t nextDigity = y >> place;
        uint64_t nextDigitz = z >> place;
        nextDigitx = nextDigitx & 1;  // sets all but the digit in question to 0
        nextDigity = nextDigity & 1;
        nextDigitz = nextDigitz & 1;

        // Put each digit at the new places
        uint64_t placedBitx = nextDigitx << nextPlace;

        if (nextPlace >= 63) { break; }  // If next place >= 63, it those bits would be truncated anyways - no reason to calculate
        uint64_t placedBity = nextDigity << (nextPlace + 1);  // have to add 1 because y was shifted 1 bit out of place compared to x

        if (nextPlace >= 62) { break; }  // If next place >= 62, it those bits would be truncated anyways - no reason to calculate
        uint64_t placedBitz = nextDigitz << (nextPlace + 2);

        newNum |= placedBitx;  // use OR to add new x bit to new number
        newNum |= placedBity;
        newNum |= placedBitz;
    }
    // Truncates newNum to 64 bits when returned
    return static_cast<uint64_t>(newNum);
}