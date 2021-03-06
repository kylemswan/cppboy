#include "MMU.hpp"
#include "CPU.hpp"

int main(int argc, char **argv) {
    // screen and input to be initialised here ...

    // initialise MMU and read in the cartridge
    MMU mmu;

    // initialise the CPU and start running instructions!
    CPU cpu;
    cpu.reset();
    cpu.connectMMU(&mmu);
    cpu.run();

    return 0;
}
