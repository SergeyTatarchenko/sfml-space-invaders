/**
 * @file canvas.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <memory>
#include <random>
#include "items.hpp"

struct GameControl
{
    uint32_t event_counter;
    bool     player_reload;
    bool     left_pressed;
    bool     right_pressed;
};

struct GameConfig
{
    unsigned int invader_shot_period;
    float player_speed;
    float enemy_ship_speed;
    float invader_speed;
    float shell_speed;
};

struct GameStatus
{
    sf::Text score_text;
    int score;
};

struct ResourceManager
{
    sf::Texture player;
    sf::Texture enemy_type_1;
    sf::Texture enemy_type_2;
    sf::Texture enemy_type_3;
    sf::Texture enemy_ship;
    sf::Texture shell;
    sf::Font game_font;
};

class Canvas
{
    public:
        /// @brief default constructor
        /// @param width canvas initial width in pixels
        /// @param height canvas initial height in pixels
        /// @param framerate canvas initial framerate
        Canvas(const unsigned int width, unsigned int height, const unsigned int framerate);
        void gameTask();
        /// @brief spawn enemies on canvas
        void spawnEnemies();
        
    private:
        /// @brief vector with invaders
        std::vector<Invader> enemies;
        /// @brief vector with invader ships
        std::vector<InvaderShip> enemyShips;
        /// @brief vector with shell instances
        std::vector<Shell> bullets;
        /// @brief pointer to player ship
        std::unique_ptr<PlayerShip> player;        
        /// @brief pointer to SFML window
        std::unique_ptr<sf::RenderWindow> window;
        /// @brief actual view grid
        sf::Vector2f grid; 
        /// @brief struct with game control items
        GameControl game_control;
        /// @brief struct with game configuration
        GameConfig game_config;
        /// @brief struct with game status
        GameStatus game_status;
        /// @brief random number generator instance
        std:: minstd_rand randomizer;
        /// @brief struct with textures for game objects 
        ResourceManager resource_manager;
        /// @brief spawn invaders on the canvas
        void spawnInvaders();
        /// @brief game event generator
        void generateGameEvent();
        /// @brief textures loading
        void loadTextures();
        /// @brief calculate items speed based on actual framerate
        /// @param framerate expected framerate
        void calculateItemsSpeed(const unsigned int framerate);
        /// @brief render items on canvas according to their actual state
        void updateCanvas();
        /// @brief update items on canvas according to their trajectory
        void updateItemsPosition();
        /// @brief control all items on canvas and remove them if they leave visible space
        void controlItemsPosition();
        /// @brief check for collision between sprites on the canvas
        void checkCollision();
        /// @brief SFML event executor for windowEventHandler
        /// @param event reference to actual captured event
        void executeEvent(const sf::Event& event);
        /// @brief generate a new shell on the canvas
        /// @param rectangle reference to object rectangle from which shell be generated (shot amimation)
        /// @param shell_type shell type (who shot this shell)
        void objectShot(const sf::FloatRect& rectangle, const ShellType shell_type);
};      

#endif //CANVAS_H
