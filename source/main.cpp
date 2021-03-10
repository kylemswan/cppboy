#include "cpu.hpp"
#include "mmu.hpp"
#include "debug.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./cppboy <ROM>\n";
        return -1;
    }

    MMU mmu;
    mmu.loadROM(argv[1]);

    CPU cpu;
    cpu.reset();
    cpu.connectMMU(&mmu);

    DebugPanel d;
    d.drawDebugInfo(cpu.getState());

    while (!d.shouldClose()) {
        // execute a CPU op when the spacebar is pressed and update debug text
        if (d.spacebarPressed()) {
            cpu.run();
            d.drawDebugInfo(cpu.getState());

            // delay thread for 100 milliseconds to prevent repeated input
            d.pauseInput(100);
        }
    }

    return 0;
}
