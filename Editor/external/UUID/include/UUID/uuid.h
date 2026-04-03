#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <array>
#include <cstdint>

class UUID {
private:
    std::array<uint8_t, 16> data;

public:
    UUID() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis;

        // Generate 128 bits of random data
        uint64_t* p = reinterpret_cast<uint64_t*>(data.data());
        p[0] = dis(gen);
        p[1] = dis(gen);

        // Set version (4) and variant bits for UUID v4
        data[6] = (data[6] & 0x0F) | 0x40;  // Version 4
        data[8] = (data[8] & 0x3F) | 0x80;  // Variant 1
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (int i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10)
                ss << '-';
            ss << std::setw(2) << static_cast<int>(data[i]);
        }

        return ss.str();
    }
};
