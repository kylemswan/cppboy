#include "cpu.hpp"
#include "mmu.hpp"
#include "debug.hpp"

#include <iostream>
#include <string>

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


    if (argc == 3 && std::string(argv[2]) == "--debug") {

        // debug panel that steps through each op
        DebugPanel d;
        d.drawDebugInfo(cpu.getState());

        while (!d.shouldClose()) {
            // execute a CPU op when the spacebar is pressed
            if (d.spacebarPressed()) {
                cpu.run();
                d.drawDebugInfo(cpu.getState());

                // delay thread for 100 milliseconds to prevent repeated input
                d.pauseInput(100);
            }
        }

    } else {
        
        // main emulation loop that runs at 60 frames per second
        sf::Clock timer;

        while (true) {
            // CPU executes 4194304 cycles per second, therefore 4194304 / 60
            // = 69905 per frame
            while (cpu.getCycles() < 69905) {
                cpu.run();
            }

            // draw graphics here...

            while (timer.getElapsedTime().asSeconds() < (float)1 / 60) {
                // wait as necessary ...
            }
            timer.restart();
        }
        
    }

    return 0;
}
