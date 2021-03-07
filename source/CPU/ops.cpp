#include "CPU.hpp"

// loads and move instructions
void CPU::LD(u8 &target, u8 val) {
    target = val;
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

void CPU::LDrr(u8 &hi, u8 &lo, u16 val) {
    setPair(hi, lo, val);
}

void CPU::LDsp(u16 val) {
    SP = val;
}

void CPU::LDD(u8 &target, u8 val) {
    target = val;
    DECrr(H, L);
}

void CPU::LDI(u8 &target, u8 val) {
    target = val;
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

// logic instructions
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

void CPU::DEC(u8 &target) {
    int result = target - 1;
    setFlag(FLAG_Z, result == 0);
    setFlag(FLAG_N, 1);
    setFlag(FLAG_H, (target & 0xF) < 1);
    target = (u8)result;
}

void CPU::DECrr(u8 &hi, u8 &lo) {
    u16 val = getPair(hi, lo);
    setPair(hi, lo, val - 1);
}

void CPU::DECsp() {
    SP -= 1;
}

void CPU::INC(u8 &target) {
    int result = target + 1;
    setFlag(FLAG_Z, result & 0xFF);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, ((target & 0xF) + 1) & 0x10);
    target = (u8)result;
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

// bit rotating and shifting instructions
void CPU::RL(u8 &target, bool circular) {
    u8 B7 = target & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A <<= 1;
    if (circular) {
        target |= B7 >> 7;
    } else {
        target |= flagC;
    }
    setFlag(FLAG_Z, target == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

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

void CPU::RR(u8 &target, bool circular) {
    u8 B7 = target & 0x80;
    u8 flagC = getFlag(FLAG_C);
    A >>= 1;
    if (circular) {
        target |= B7 >> 7;
    } else {
        target |= flagC;
    }
    setFlag(FLAG_Z, target == 0);
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

void CPU::SLA(u8 &target) {
    u8 B7 = target >> 7;
    target <<= 1;
    setFlag(FLAG_Z, target == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B7);
}

void CPU::SRA(u8 &target) {
    u8 B0 = target & 1;
    u8 B7 = target & 0x80;
    target >>= 1;
    target |= B7;
    setFlag(FLAG_Z, target == 0);
    setFlag(FLAG_N, 0);
    setFlag(FLAG_H, 0);
    setFlag(FLAG_C, B0);
}

void CPU::SRL(u8 &target) {
    u8 B0 = target & 1;
    target >>= 1;
    setFlag(FLAG_Z, target == 0);
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

void CPU::RES(int bit, u8 &target) {
    target &= ~(1 << bit);
}

void CPU::SET(int bit, u8 &target) {
    target |= (1 << bit);
}

void CPU::SWAP(u8 &target) {
    u8 temp = target;
    target = ((temp & 0xF0) >> 4) | ((temp & 0x0F) << 4);
    setFlag(FLAG_Z, target == 0);
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

// special op to catch all unimplemented or missed ops in development
void CPU::XXX() {

}
