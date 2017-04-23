
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
#include "Enemy.hpp"
#include "Bubble.hpp"
#include "SoundPlayer.hpp"

void clearScreen(sf::Uint8* pixels) {
    for(int i = 0; i < screenWidth * screenHeight * 4; i++) pixels[i] = 0xFF;
}

float abssin(float n) {
    return fabs(sin(n));
}

void nextWave(int n, Entity::container& entities, Player* player) {
    soundPlayer.play(Sound::RDY);

    switch(n) {
        case 0:
            entities.emplace_back(new Enemy({40.f, 100.f}, player));
            break;

        case 1:
            entities.emplace_back(new Enemy({40.f, 100.f}, player));
            entities.emplace_back(new Enemy({200.f, 100.f}, player));
            break;

        default:
            for(int i = 0; i < n + 1; i++) {
                entities.emplace_back(new Enemy({0.f + rand() % screenWidth, 0.f + rand() % screenHeight}, player));
            }
            break;
    }
}

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(screenWidth * screenScale, screenHeight * screenScale), "Gemu");

#ifdef VSYNC
    window.setVerticalSyncEnabled(true);
#endif

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


    int wave = 0;
    std::deque<int> fpsAvg;
    sf::Time waveTime = sf::seconds(2);
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

        if(waveTime > sf::Time::Zero && waveTime - dt <= sf::Time::Zero) {
            nextWave(wave++, entities, player);
        }

        soundPlayer.tick(sf::Time::Zero);
        waveTime -= dt;

        // Ensure framerate
        acc += dt;
        if(acc >= frameTime) {
            acc -= frameTime;

            sf::Time fTime = (player->state == Player::FREE) ? frameTime : frameTime / drawSlowdown;
            std::vector<Entity*> cop;
            for(auto& ptr : entities) cop.push_back(ptr.get());
            for(auto ptr : cop) ptr->tick(fTime, entities);

            // We don't want to remove player, reset game instead
            if(player->dead) {
                for(auto& ptr : entities)
                    if(ptr->id == Entity::ENEMY) ptr->dead = true;

                player->reset();
                wave = 0;
                waveTime = sf::seconds(2);
            }

            // Remove dead entities
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                                         [](auto& ptr){ return ptr->dead; }),
                            entities.end());


            int enemyCount = 0;
            for(auto& ptr : entities) enemyCount += ptr->id == Entity::ENEMY ? 1 : 0;

            if(enemyCount == 0 && waveTime < sf::Time::Zero) waveTime = sf::seconds(2.f);

            // Update camera
            //camera.left = std::max(0.f, std::min(cameraFocus->pos.x - screenWidth / 2, maxCameraX - screenWidth));
            //camera.top = std::min(cameraFocus->pos.y - screenHeight / 2, maxCameraY - screenHeight);
            clearScreen(pixels);
            for(auto& ptr : entities) ptr->render(pixels, camera);

        }



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
