#include "CPU.hpp"

void CPU::LDaddr(u16 addr, u8 val) {
    mmu->write8(addr, val);
}

void CPU::LDaddrsp(u16 addr, u16 val) {
    mmu->write16(addr, SP);
}

void CPU::LDhl(u16 val) {
    setPair(H, L, val);
    setFlag(FLAG_Z, 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 1);
    setFlag(FLAG_C, 0);
}

void CPU::LDr(u8 &reg, u8 val) {
    reg = val;
}

void CPU::LDrr(u8 &hi, u8 &lo, u16 val) {
    setPair(hi, lo, val);
}

void CPU::LDsp(u16 val) {
    SP = val;
}

void CPU::LDDaddr(u16 addr, u8 val) {
    mmu->write8(addr, val);
    DECrr(H, L);
}

void CPU::LDDr(u8 &reg, u8 val) {
    reg = val;
    DECrr(H, L);
}

void CPU::LDIaddr(u16 addr, u8 val) {
    mmu->write8(addr, val);
    INCrr(H, L);
}

void CPU::LDIr(u8 &reg, u8 val) {
    reg = val;
    INCrr(H, L);
}

void CPU::PUSH(u8 hi, u8 lo) {
    SP -= 2;
    mmu->write16(SP, getPair(hi, lo));
}

void CPU::POP(u8 &hi, u8 &lo) {
    setPair(hi, lo, mmu->read16(SP));
    SP += 2;
}

void CPU::AND(u8 val) {
    A &= val;
    setFlag(FLAG_Z, A == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 1);
    setFlag(FLAG_C, 0);
}

void CPU::CCF() {
    FLAGS ^= FLAG_C;
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
}

void CPU::CP(u8 val) {
    u8 result = A - val;
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (A & 0xF) < (val & 0xF));
    setFlag(FLAG_C, A < val);
}

void CPU::CPL() {
    A = ~A;
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, 1);
}

void CPU::DAA() {

}

void CPU::OR(u8 val) {
    A |= val;
    setFlag(FLAG_Z, A == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, 0);
}

void CPU::SCF() {
    setFlag(FLAG_N, 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_C, 1);
}

void CPU::XOR(u8 val) {
    A ^= val;
    setFlag(FLAG_Z, A == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, 0);
}

// arithmetic instructions
void CPU::ADC(u8 val) {
    u8 flagC = getFlag(FLAG_C);
    int result = A + val + flagC;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((A + 0xF) + (val & 0xF) + (flagC + 0xF)) & 0x10);
    setFlag(FLAG_C, result > 0xFF);
    A = (u8)result;
}

void CPU::ADDa(u8 val) {
    int result = A + val;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((A & 0xF) + (val & 0xF)) & 0x10);
    setFlag(FLAG_C, result > 0xFF);
    A = (u8)result;
}

void CPU::ADDhl(u16 val) {
    int result = getPair(H, L) + val;
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((getPair(H, L) & 0x0FFF) + (val & 0x0FFF)) & 0x1000);
    setFlag(FLAG_C, result > 0xFFFF);
    setPair(H, L, (u16)result);
}

void CPU::ADDsp(s8 val) {
    int result = SP + val;
    setFlag(FLAG_Z, 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((SP & 0xF) + (val & 0xF)) & 0x10);
    setFlag(FLAG_C, ((SP & 0xFF) + (val & 0xFF)) & 0x0100);
    SP = (u16)result;
}

void CPU::DECaddr(u16 addr) {
    u8 val = mmu->read8(addr);
    int result = val - 1;
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (val & 0xF) < 1);
    mmu->write8(addr, (u8)result);
}

void CPU::DECr(u8 &reg) {
    int result = reg - 1;
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (reg & 0xF) < 1);
    reg = (u8)result;
}

void CPU::DECrr(u8 &hi, u8 &lo) {
    u16 val = getPair(hi, lo);
    setPair(hi, lo, val - 1);
}

void CPU::DECsp() {
    SP -= 1;
}

void CPU::INCaddr(u16 addr) {
    u8 val = mmu->read8(16);
    int result = val + 1;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((val & 0xF) + 1) & 0x10);
    mmu->write8(addr, (u8)result);
}

void CPU::INCr(u8 &reg) {
    int result = reg + 1;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((reg & 0xF) + 1) & 0x10);
    reg = (u8)result;
}

void CPU::INCrr(u8 &hi, u8 &lo) {
    u16 val = getPair(hi, lo);
    setPair(hi, lo, val + 1);
}

void CPU::INCsp() {
    SP += 1;
}

void CPU::SBC(u8 val) {
    u8 flagC = getFlag(FLAG_C);
    int result = A - val - flagC;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (A & 0xF) - (val & 0xF) - (flagC & 0xF) < 0);
    setFlag(FLAG_C, result < 0);
    A = (u8)result;
}

void CPU::SUB(u8 val) {
    int result = A - val;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (A & 0xF) < (val & 0xF));
    setFlag(FLAG_C, result < 0);
    A = (u8)result;
}

// jump and return instructions
void CPU::CALL(u16 addr) {
    PUSH((addr & 0xFF00) >> 8, addr & 0x00FF);
    JP(addr);
}

void CPU::CALLcond(u16 addr, bool cond) {
    if (cond) {
        CALL(addr);
        cycles += 12;
    }
}

void CPU::JP(u16 addr) {
    PC = addr;
}

void CPU::JPcond(u16 addr, bool cond) {
    if (cond) {
        JP(addr);
        cycles += 4;
    }
}

void CPU::JR(s8 val) {
    PC += val;
}

void CPU::JRcond(s8 val, bool cond) {
    if (cond) {
        JR(val);
        cycles += 4; 
    }
}

void CPU::RET() {
    u8 hi, lo;
    POP(hi, lo);
    JP(getPair(hi, lo));
}

void CPU::RETcond(bool cond) {
    if (cond) {
        RET();
        cycles += 12;
    }
}

void CPU::RETI() {
    RET();
    EI();
}

// bit shifting instructions
void CPU::RLa(bool circular) {
    u8 B7 = A & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A <<= 1;
    if (circular) {
        A |= B7 >> 7;
    } else {
        A |= flagC;
    }
    setFlag(FLAG_Z, 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::RLaddr(u16 addr, bool circular) {
    u8 val = mmu->read8(addr);
    u8 B7 = val & 0x80;
    u8 flagC = getFlag(FLAG_C);
    val <<= 1;
    if (circular) {
        val |= B7 >> 7;
    } else {
        val |= flagC;
    }
    setFlag(FLAG_Z, val == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
    mmu->write8(addr, val);
}

void CPU::RLr(u8 &reg, bool circular) {
    u8 B7 = reg & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A <<= 1;
    if (circular) {
        reg |= B7 >> 7;
    } else {
        reg |= flagC;
    }
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::RRa(bool circular) {
    u8 B7 = A & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A >>= 1;
    if (circular) {
        A |= B7 >> 7;
    } else {
        A |= flagC;
    }
    setFlag(FLAG_Z, 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::RRaddr(u16 addr, bool circular) {
    u8 val = mmu->read8(addr);
    u8 B7 = val & 0x80;
    u8 flagC = getFlag(FLAG_C);
    val >>= 1;
    if (circular) {
        val |= B7 >> 7;
    } else {
        val |= flagC;
    }
    setFlag(FLAG_Z, val == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
    mmu->write8(addr, val);
}

void CPU::RRr(u8 &reg, bool circular) {
    u8 B7 = reg & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A >>= 1;
    if (circular) {
        reg |= B7 >> 7;
    } else {
        reg |= flagC;
    }
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::SLAaddr(u16 addr) {
    u8 val = mmu->read8(addr);
    u8 B7 = val >> 7;
    val <<= 1;
    mmu->write8(addr, val);
    setFlag(FLAG_Z, val == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::SLAr(u8 &reg) {
    u8 B7 = reg >> 7;
    reg <<= 1;
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::SRAaddr(u16 addr) {
    u8 val = mmu->read8(addr);
    u8 B0 = val & 1;
    u8 B7 = val & 0x80;
    val >>= 1;
    val |= B7;
    mmu->write8(addr, val);
    setFlag(FLAG_Z, val == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B0);
}

void CPU::SRAr(u8 &reg) {
    u8 B0 = reg & 1;
    u8 B7 = reg & 0x80;
    reg >>= 1;
    reg |= B7;
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B0);
}

void CPU::SRLaddr(u16 addr) {
    u8 val = mmu->read8(addr);
    u8 B0 = val & 1;
    val >>= 1;
    mmu->write8(addr, val);
    setFlag(FLAG_Z, val == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B0);
}

void CPU::SRLr(u8 &reg) {
    u8 B0 = reg & 1;
    reg >>= 1;
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B0);
}

// bit setting and clearing
void CPU::BIT(int bit, u8 val) {
    u8 result = (val >> bit) & 1;
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 1);
}

void CPU::RESaddr(int bit, u16 addr) {
    u8 val = mmu->read8(addr);
    val &= ~(1 << bit);
    mmu->write8(addr, val);
}

void CPU::RESr(int bit, u8 &reg) {
    reg &= ~(1 << bit);
}

void CPU::SETaddr(int bit, u16 addr) {
    u8 val = mmu->read8(addr);
    val |= (1 << bit);
    mmu->write8(addr, val);
}

void CPU::SETr(int bit, u8 &reg) {
    reg |= (1 << bit);
}

void CPU::SWAPaddr(u16 addr) {
    u8 temp = mmu->read8(addr);
    u8 result = ((temp & 0xF0) >> 4) | ((temp & 0x0F) << 4);
    mmu->write8(addr, result);
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, 0);
}

void CPU::SWAPr(u8 &reg) {
    u8 temp = reg;
    reg = ((temp & 0xF0) >> 4) | ((temp & 0x0F) << 4);
    setFlag(FLAG_Z, reg == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, 0);
}

// control instructions
void CPU::DI() {
    intsEnabled = false;
}

void CPU::EI() {
    intsEnabled = true;
}

void CPU::HALT() {
    // should wait for an interrupt
    running = false;
}

void CPU::NOP() {
    // do nothing!!
}

void CPU::RST(u16 addr) {
    PUSH((PC & 0xFF00) >> 8, PC & 0x00FF);
    JP(addr);
}

void CPU::STOP() {
    // should wait for a key press
    running = false;
}

void CPU::XXX() {

}
