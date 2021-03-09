#include "cpu.hpp"

void CPU::reset() {
    flagZ = flagN = flagH = flagC = true;
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

std::string CPU::getState() {
    // get the value of the next opcode
    u8 currentOp = mmu->read8(PC);
    u8 nextOp = mmu->read8(PC + getPCOffset(currentOp));

    // construct formatted string displaying CPU information
    std::stringstream s;
    s << "OP " << Utils::formatHex(nextOp, 2) << "\n"
      << "A  " << Utils::formatHex(A, 2) << "\n"
      << "B  " << Utils::formatHex(B, 2) << "\n"
      << "C  " << Utils::formatHex(C, 2) << "\n"
      << "D  " << Utils::formatHex(D, 2) << "\n"
      << "E  " << Utils::formatHex(E, 2) << "\n"
      << "H  " << Utils::formatHex(H, 2) << "\n"
      << "L  " << Utils::formatHex(L, 2) << "\n"
      << "F  " << flagZ << flagN << flagH << flagC << "\n"
      << "PC " << Utils::formatHex(PC, 4) << "\n"
      << "SP " << Utils::formatHex(SP, 4) << "\n";
    return s.str();
}

void CPU::setZNHC(bool fZ, bool fN, bool fH, bool fC) {
    setZNH(fZ, fN, fH);
    flagC = fC;
}

void CPU::setZNH(bool fZ, bool fN, bool fH) {
    flagZ = fZ;
    flagN = fN;
    flagH = fH;
}

void CPU::setNHC(bool fN, bool fH, bool fC) {
    flagN = fN;
    flagH = fH;
    flagC = fC;
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
        // CB opcodes that end in '6' or 'E' take 16 cycles, all others take 8
        u8 cb = mmu->read8(PC + 1);
        u8 n2 = cb & 0x0F;
        return (n2 == 0x06) || (n2 == 0x0E) ? 16 : 8;
    } else {
        return cycleCount[op];
    }
}
