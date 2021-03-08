#include "CPU.hpp"

// loads and move instructions
void CPU::LD(u8 &target, u8 val) {
    target = val;
}

void CPU::LDaddrsp(u16 addr) {
    mmu->write16(addr, SP);
}

void CPU::LDhl(u16 val) {
    setPair(H, L, val);
    setZNHC(false, false, true, false);
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

void CPU::PUSHaf() {
    // construct a u8 corresponding to the flag states then PUSH
    u8 FLAGS = (flagZ << 7) | (flagN << 6) | (flagH << 5) | (flagC << 4);
    PUSH(A, FLAGS);
}

void CPU::POP(u8 &hi, u8 &lo) {
    setPair(hi, lo, mmu->read16(SP));
    SP += 2;
}

void CPU::POPaf() {
    // set A to the hi byte and flag states corresponding to the lo byte
    A = mmu->read8(SP + 1);
    u8 FLAGS = mmu->read8(SP);
    setZNHC(FLAGS >> 7, (FLAGS >> 6) & 1, (FLAGS >> 5) & 1, (FLAGS >> 4) & 1);
    SP += 2;
}

// logic instructions
void CPU::AND(u8 val) {
    A &= val;
    setZNHC(!A, false, true, false);
}

void CPU::CCF() {
    flagC = !flagC;
    flagN = flagH = false;
}

void CPU::CP(u8 val) {
    u8 res = A - val;
    setZNHC(!res, true, (A & 0xF) < (val & 0xF), res > A);
}

void CPU::CPL() {
    A = ~A;
    flagN = flagH = true;
}

void CPU::DAA() {

}

void CPU::OR(u8 val) {
    A |= val;
    setZNHC(!A, false, false, false);
}

void CPU::SCF() {
    flagC = true;
    flagN = flagH = false;
}

void CPU::XOR(u8 val) {
    A ^= val;
    setZNHC(!A, false, false, false);
}

// arithmetic instructions
void CPU::ADC(u8 val) {
    u8 add = val + flagC;
    u8 res = A + add;
    setZNHC(!res, false, (A & 0xF) + (add & 0xF) > 0xF, res < A);
    A = res;
}

void CPU::ADD(u8 val) {
    u8 res = A + val;
    setZNHC(!res, false, (A & 0xF) + (val & 0xF) > 0xF, res < A);
    A = res;
}

void CPU::ADDhl(u16 val) {
    u16 HL = getPair(H, L);
    u16 res = HL + val;
    flagN = false;
    flagH = (HL & 0x0FFF) + (val & 0x0FFF) > 0x0FFF;
    flagC = res < HL;
    setPair(H, L, res);
}

void CPU::ADDsp(s8 val) {
    u16 res;
    if (val < 0) {
        res = SP - (u8)val;
        setZNHC(false, false, (SP & 0xF) < ((u8)val & 0xF), res > SP);
    } else {
        res = SP + (u8)val;
        setZNHC(false, false, (SP & 0xF) + ((u8)val & 0xF) > 0xF, res < SP);
    }
    SP = res;
}

void CPU::DEC(u8 &target) {
    u8 res = target - 1;
    flagZ = !res;
    flagN = true;
    flagH = (target & 0xF) < 1;
    target = res;
}

void CPU::DECrr(u8 &hi, u8 &lo) {
    u16 val = getPair(hi, lo);
    setPair(hi, lo, val - 1);
}

void CPU::DECsp() {
    SP -= 1;
}

void CPU::INC(u8 &target) {
    u8 res = target + 1;
    flagZ = !res;
    flagN = false;
    flagH = (target & 0xF) + 1 > 0xF;
    target = res;
}

void CPU::INCrr(u8 &hi, u8 &lo) {
    u16 val = getPair(hi, lo);
    setPair(hi, lo, val + 1);
}

void CPU::INCsp() {
    SP += 1;
}

void CPU::SBC(u8 val) {
    u8 sub = val + flagC;
    u8 res = A - sub;
    setZNHC(!res, true, (A & 0xF) < (sub & 0xF), res > A);
    A = res;
}

void CPU::SUB(u8 val) {
    u8 res = A - val;
    setZNHC(!res, true, (A & 0xF) < (val & 0xF), res > A);
    A = res;
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
    A <<= 1;
    if (circular) {
        target |= B7 >> 7;
    } else {
        target |= flagC;
    }
    setZNHC(!target, false, false, B7);
}

void CPU::RLa(bool circular) {
    // call the standard RL op on A but reset flag Z
    RL(A, circular);
    flagZ = false;
}

void CPU::RR(u8 &target, bool circular) {
    u8 B7 = target & 0x80;
    A >>= 1;
    if (circular) {
        target |= B7 >> 7;
    } else {
        target |= flagC;
    }
    setZNHC(!target, false, false, B7);
}

void CPU::RRa(bool circular) {
    // call the standard RR op on register A but reset flag Z
    RR(A, circular);
    flagZ = false;
}

void CPU::SLA(u8 &target) {
    u8 B7 = target >> 7;
    target <<= 1;
    setZNHC(!target, false, false, B7);
}

void CPU::SRA(u8 &target) {
    u8 B0 = target & 1;
    u8 B7 = target & 0x80;
    target >>= 1;
    target |= B7;
    setZNHC(!target, false, false, B0);
}

void CPU::SRL(u8 &target) {
    u8 B0 = target & 1;
    target >>= 1;
    setZNHC(!target, false, false, B0);
}

// bit setting and clearing
void CPU::BIT(int bit, u8 val) {
    u8 result = (val >> bit) & 1;
    flagZ = !result;
    flagN = false;
    flagH = true;
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
    setZNHC(!target, false, false, false);
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
