#pragma once

#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"
#include "../Engine/GameObject.h"
#include "../Entittes/Player.h"

namespace cli
{

class KeyboardInterface
{
 private:
    sf::Vector2f _current_position;
    uint32_t _direction;

 public:
    uint32_t get_direction() const
    {
        uint32_t direction = 320;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            direction = config::Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            direction = config::Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            direction = config::Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            direction = config::Down;
        }
        return direction;
    }


 public:
    KeyboardInterface() :
        _direction(320),
        _current_position(0, 0)
    {}
};

class Manager
{
 public:
    Manager(uint32_t x_resolution, uint32_t y_resolution, std::string &&window_name);

    sf::Packet make_step(sf::Packet &packet);

    int process_packet(sf::Packet &packet);

    bool is_active();

    void draw();

 private:

    // Window parameters
    sf::RenderWindow _window;
    sf::Vector2u _resolution;
    std::string _window_name;
    bool _is_window_focused;
    bool _is_window_opened;

    uint32_t _current_num_of_clients;
    KeyboardInterface keyboard;

    std::vector<Player> _objects;
};

}