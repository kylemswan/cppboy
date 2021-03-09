#include "utils.hpp"

void Utils::setPair(u8 &hi, u8 &lo, u16 val) {
    hi = (val & 0xFF00) >> 8;
    lo = val & 0x00FF;
}

u16 Utils::getPair(u8 hi, u8 lo) {
    return (hi << 8) | lo;
}

u8 Utils::getHi(u16 val) {
    return (val & 0xFF00) >> 8;
}

u8 Utils::getLo(u16 val) {
    return val & 0x00FF;
}

std::string Utils::formatHex(int val, int width) {
    std::stringstream s;
    s << std::uppercase 
      << std::setw(width)
      << std::setfill('0')
      << std::hex
      << val;
    return s.str();
}