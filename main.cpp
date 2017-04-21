
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Consts.hpp"
#include "Entity.hpp"

void setPixel(sf::Uint8* pixels, int x, int y, sf::Color color) {
    if(x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) return;

    int i = 4 * (y * screenWidth + x);
    pixels[i] = color.r;
    pixels[i+1] = color.g;
    pixels[i+2] = color.b;
    pixels[i+3] = color.a;
}

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

    std::vector<Entity::ptr> entities;

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

            for(auto& ptr : entities) ptr->tick(frameTime);
            for(auto& ptr : entities) ptr->render(pixels);
        }

        window.clear(sf::Color::White);

        // draw everything here...
        screen.update(pixels);
        window.draw(screenContainer);

        // end the current frame
        window.display();

        clearScreen(pixels);
    }

    delete[] pixels;

    return 0;
}
