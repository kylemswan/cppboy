#include "debug.hpp"

DebugPanel::DebugPanel() {
    win.create(sf::VideoMode(640, 320), "cppboy");
    font.loadFromFile("assets/bitfont.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::White);
}

bool DebugPanel::shouldClose() {
    while (win.pollEvent(event)) {
        // check for 'X' button clicks
        if (event.type == sf::Event::Closed) {
            return true;
        }
        // check for 'escape' presses
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            return true;
        }
    }
    return false;
}

bool DebugPanel::spacebarPressed() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void DebugPanel::drawDebugInfo(std::string cpuState) {
    win.clear(sf::Color::Black);
    text.setString(cpuState);
    win.draw(text);
    win.display();
}

void DebugPanel::pauseInput(int ms) {
    sf::sleep(sf::milliseconds(ms));
}
