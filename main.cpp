
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Consts.hpp"
#include "Entity.hpp"
#include "Player.hpp"

void clearScreen(sf::Uint8* pixels) {
    for(int i = 0; i < screenWidth * screenHeight * 4; i++) pixels[i] = 0xFF;
}

float abssin(float n) {
    return fabs(sin(n));
}

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(screenWidth * screenScale, screenHeight * screenScale), "Gemu");

    window.setVerticalSyncEnabled(false);

    sf::Texture screen;
    if(!screen.create(screenWidth, screenHeight)) {
        std::cout << "Creation of screen texture failed!" << std::endl;
        return 1;
    }

    sf::Uint8 *pixels = new sf::Uint8[screenWidth * screenHeight * 4];

    for(int i = 0; i < screenWidth * screenHeight * 4; i++)
        pixels[i] = 0xff;

    screen.update(pixels);

    sf::Sprite screenContainer;
    screenContainer.setTexture(screen);
    screenContainer.setScale(1.0 * screenScale, 1.0 * screenScale);

    sf::FloatRect camera(0, 0, screenWidth, screenHeight);

    std::vector<Entity::ptr> entities;

    entities.emplace_back(new Player({40, 40}));

    sf::Time frameTime = sf::seconds(1.f / frameRate);
    sf::Time acc = sf::Time::Zero;
    sf::Clock clock;
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }


        // Ensure framerate
        acc += clock.restart();
        if(acc >= frameTime) {
            acc -= frameTime;

            for(auto& ptr : entities) ptr->tick(frameTime, entities);

            clearScreen(pixels);
            for(auto& ptr : entities) ptr->render(pixels, camera);
        }

        window.clear(sf::Color::White);

        // draw everything here...
        screen.update(pixels);
        window.draw(screenContainer);

        // end the current frame
        window.display();
    }

    delete[] pixels;

    return 0;
}
