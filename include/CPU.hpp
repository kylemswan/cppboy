#ifndef CPU_HPP
#define CPU_HPP

#include "types.hpp"
#include "MMU.hpp"

class CPU {
    public:
    void reset();
    void connectMMU(MMU *mmu);
    void run();

    private:
    u8 A, B, C, D, E, H, L, FLAGS;
    u16 SP, PC;

    MMU *mmu;

    bool intsEnabled;
    bool running;

    // utility functions for dealing with register pairs
    void setPair(u8 &hi, u8 &lo, u16 val);
    u16 getPair(u8 hi, u8 lo);

    // flag logic
    enum Flag {
        FLAG_Z = 1 << 7,
        FLAG_N = 1 << 6,
        FLAG_H = 1 << 5, 
        FLAG_C = 1 << 4
    };
    void setFlag(Flag f, bool state);
    bool getFlag(Flag f);

    // timing logic (unimplemented right now apart from just adding cycles...)
    int cycles;

    // dispatch functions
    void exec(u8 opcode);
    void execCB(u8 opcode);

    // loads and move instructions
    void LDaddr(u16 addr, u8 val);
    void LDaddrsp(u16 addr, u16 val);
    void LDhl(u16 val);
    void LDr(u8 &reg, u8 val);
    void LDrr(u8 &hi, u8 &lo, u16 val);
    void LDsp(u16 val);

    void LDDaddr(u16 addr, u8 val);
    void LDDr(u8 &reg, u8 val);

    void LDIaddr(u16 addr, u8 val);
    void LDIr(u8 &reg, u8 val);
    
    void PUSH(u8 hi, u8 lo);
    void POP(u8 &hi, u8 &lo);

    // logic instructions
    void AND(u8 val);
    void CCF();
    void CP(u8 val);
    void CPL();
    void DAA();
    void OR(u8 val);
    void SCF();
    void XOR(u8 val);

    // arithmetic instructions
    void ADC(u8 val);
    void ADDa(u8 val);
    void ADDhl(u16 val);
    void ADDsp(s8 val);

    void DECaddr(u16 addr);
    void DECr(u8 &reg);
    void DECrr(u8 &hi, u8 &lo);
    void DECsp();

    void INCaddr(u16 addr);
    void INCr(u8 &reg);
    void INCrr(u8 &hi, u8 &lo);
    void INCsp();

    void SBC(u8 val);
    void SUB(u8 val);

    // jump and return instructions
    void CALL(u16 addr);
    void CALLcond(u16 addr, bool cond);

    void JP(u16 addr);
    void JPcond(u16 addr, bool cond);

    void JR(s8 val);
    void JRcond(s8 val, bool cond);

    void RET();
    void RETcond(bool cond);
    void RETI();

    // bit shifting instructions
    void RLa(bool circular);
    void RLaddr(u16 addr, bool circular);
    void RLr(u8 &reg, bool circular);

    void RRa(bool circular);
    void RRaddr(u16 addr, bool circular);
    void RRr(u8 &reg, bool circular);

    void SLAaddr(u16 addr);
    void SLAr(u8 &reg);

    void SRAaddr(u16 addr);
    void SRAr(u8 &reg);

    void SRLaddr(u16 addr);
    void SRLr(u8 &reg);

    // bit setting and clearing
    void BIT(int bit, u8 val);

    void RESaddr(int bit, u16 addr);
    void RESr(int bit, u8 &reg);

    void SETaddr(int bit, u16 addr);
    void SETr(int bit, u8 &reg);

    void SWAPaddr(u16 addr);
    void SWAPr(u8 &reg);

    // control instructions
    void DI();
    void EI();
    void HALT();
    void NOP();
    void RST(u16 addr);
    void STOP();

    // special function to catch unimplemented or missed opcodes
    void XXX();

    // tables for fetching PC offsets and cycle count based on the op performed
    int getCycleCount(u8 op);
    int getPCOffset(u8 op);

    int pcOffset[0x100] = {
         1,  3,  1,  1,  1,  1,  2,  1,  3,  1,  1,  1,  1,  1,  2,  1,
         2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,
         2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,
         2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  3,  3,  3,  1,  2,  0,  1,  1,  3,  0,  3,  3,  2,  0,
         1,  1,  3,  0,  3,  1,  2,  0,  1,  1,  3,  0,  3,  0,  2,  0,
         2,  1,  2,  0,  0,  1,  2,  0,  2,  1,  3,  0,  0,  0,  2,  0,
         2,  1,  2,  1,  0,  1,  2,  0,  2,  1,  3,  1,  0,  0,  2,  0
    };

    int cycleCount[0x100] = {
         4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,
         4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,
         8, 12,  8,  8,  4,  4,  8,  4,  8,  8,  8,  8,  4,  4,  8,  4,
         8, 12,  8,  8, 12, 12, 12,  4,  8,  8,  8,  8,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
         8, 12, 12, 16, 12, 16,  8, 16,  8, 16, 12,  0, 12, 24,  8, 16,
         8, 12, 12,  0, 12, 16,  8, 16,  8, 16, 12,  0, 12,  0,  8, 16,
        12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,
        12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16
    };
};

#endif // "CPU.hpp" included