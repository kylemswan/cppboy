#include "MMU.hpp"

void MMU::write8(u16 addr, u8 data) {
    memory[addr] = data;
}

void MMU::write16(u16 addr, u16 data) {
    memory[addr] = data & 0x00FF;
    memory[addr + 1] = (data & 0xFF00) >> 8;
}

u8 MMU::read8(u16 addr) {
    return memory[addr];
}

u16 MMU::read16(u16 addr) {
    return (memory[addr + 1] << 8) | memory[addr];
}

u8 &MMU::getRef(u16 addr) {
    return memory[addr];
}
