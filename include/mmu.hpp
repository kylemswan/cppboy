#ifndef MMU_HPP
#define MMU_HPP

#include "types.hpp"

class MMU {
    public:
    void write8(u16 addr, u8 data);
    void write16(u16 addr, u16 data);

    u8 read8(u16 addr);
    u16 read16(u16 addr);

    // return a reference to the given address - only allow for writable memory
    u8 &getRef(u16 addr);

    private:
    u8 memory[0xFFFF];
};

#endif // "MMU.hpp" included
