#ifndef UTILS_HPP
#define UTILS_HPP

#include "types.hpp"

#include <string>
#include <sstream>
#include <iomanip>

namespace Utils {
    void setPair(u8 &hi, u8 &lo, u16 val);
    void setBit(u8 &target, int bit, bool state);

    u16 getPair(u8 hi, u8 lo);
    u8 getHi(u16 val);
    u8 getLo(u16 val);
    bool getBit(u8 val, int bit);

    std::string formatHex(int val, int width);
};

#endif // "utils.hpp" included
