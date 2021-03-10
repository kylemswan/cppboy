#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>

class DebugPanel {
    public:
    DebugPanel();

    bool shouldClose();
    bool spacebarPressed();
    void drawDebugInfo(std::string cpuState);
    void pauseInput(int ms);

    private:
    sf::RenderWindow win;
    sf::Event event;
    sf::Font font;
    sf::Text text;
};

#endif // "debug.hpp" included
