
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Consts.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "ParticleSystem.hpp"
#include "GoodThing.hpp"

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


    Player* player = new Player({screenWidth * 0.5f, screenHeight * 0.2f});

    sf::FloatRect camera(0, 0, screenWidth, screenHeight);
    Entity* cameraFocus = player;

    std::vector<Entity::ptr> entities;
    entities.emplace_back(ParticleSystem::getInstance());
    entities.emplace_back(player);

    entities.emplace_back(new GoodThing({100.f, 20.f}, player));
    entities.emplace_back(new GoodThing({380.f, 60.f}, player));


    std::deque<int> fpsAvg;
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

        sf::Time dt = clock.restart();

        // Ensure framerate
        acc += dt;
        if(acc >= frameTime) {
            acc -= frameTime;

            //if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) particleSystem->explode({0.5 * screenWidth + rand() % 100, 0.5 * screenHeight + rand() % 100});

            for(auto& ptr : entities) ptr->tick(frameTime, entities);

            // Update camera
            camera.left = std::max(0.f, std::min(cameraFocus->pos.x - screenWidth / 2, maxCameraX - screenWidth));
            camera.top = std::min(cameraFocus->pos.y - screenHeight / 2, maxCameraY - screenHeight);

        }

        clearScreen(pixels);
        for(auto& ptr : entities) ptr->render(pixels, camera);


        window.clear(sf::Color::White);

        // draw everything here...
        screen.update(pixels);
        window.draw(screenContainer);

        // end the current frame
        window.display();


        // Calculate framerate
        fpsAvg.push_back(1000000.f / dt.asMicroseconds());
        if(fpsAvg.size() > 100) fpsAvg.pop_front();
        float fps = 0.f;
        for(float f : fpsAvg) fps += f / 100;

        std::stringstream ss; ss << fps << "FPS";
        window.setTitle(ss.str());
    }

    delete[] pixels;

    return 0;
}
