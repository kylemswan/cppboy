#include "CPU.hpp"

void CPU::reset() {
    // set registers and timers to their initial values here ...
    PC = 0x0100;
    SP = 0xFFFE;
    cycles = 0;
}

void CPU::connectMMU(MMU *mmuIn) {
    mmu = mmuIn;
}

void CPU::run() {
    u8 op = mmu->read8(PC);
    PC += getPCOffset(op);

    // check for 0xCB prefixed opcodes
    if (op == 0xCB) {
        u8 cb = mmu->read8(PC + 1);
        execCB(cb);
    } else {
        exec(op);
    }

    cycles += getCycleCount(op);
}

void CPU::setPair(u8 &hi, u8 &lo, u16 val) {
    hi = (val & 0xFF00) >> 8;
    lo = val & 0x00FF;
}

u16 CPU::getPair(u8 hi, u8 lo) {
    return (hi << 8) | lo;
}

void CPU::setFlag(Flag f, bool state) {
    if (state) {
        FLAGS |= f;
    } else {
        FLAGS &= ~f;
    }
}

bool CPU::getFlag(Flag f) {
    return FLAGS & f;
}

int CPU::getPCOffset(u8 op) {
    if (op == 0xCB) {
        return 2;
    } else {
        return pcOffset[op];
    }
}

int CPU::getCycleCount(u8 op) {
    if (op == 0xCB) {
        u8 cb = mmu->read8(PC + 1);
        u8 n2 = cb & 0x0F;
        return (n2 == 0x06) || (n2 == 0x0E) ? 16 : 8;
    } else {
        return cycleCount[op];
    }
}
