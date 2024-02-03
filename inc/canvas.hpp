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
#include <array>
#include <memory>
#include <random>
#include "items.hpp"

struct GameControl
{
    /// @brief frame tick counter
    uint32_t event_counter;
    /// @brief label for player reload logic, when shot is not performed
    bool     player_reload;
    /// @brief left key is pressed
    bool     left_pressed;
    /// @brief right key is pressed
    bool     right_pressed;
};

struct GameConfig
{
    /// @brief shot period, compared with event_counter from GameControl
    unsigned int invader_shot_period;
    /// @brief actual player speed, adjusted with actual framerate
    float player_speed;
    /// @brief actual invader ship speed, adjusted with actual framerate
    float enemy_ship_speed;
    /// @brief actual invader speed, adjusted with actual framerate
    float invader_speed;
    /// @brief actual shell speed, adjusted with actual framerate
    float shell_speed;
};

struct GameMenuSprites 
{
    /// @brief sprite with game score
    sf::Text score;
    /// @brief sprite with player live
    sf::Sprite live; 
};

struct GameStatus
{
    /// @brief actual game score
    int score;
    /// @brief actual number of player lives
    int player_lives;
};

struct ResourceManager
{
    /// @brief texture with player image
    sf::Texture player;
    /// @brief texture with invader
    sf::Texture enemy_type_1;
    /// @brief texture with alternative invader
    sf::Texture enemy_type_2;
    /// @brief texture with alternative invader
    sf::Texture enemy_type_3;
    /// @brief texture with enemy ship
    sf::Texture enemy_ship;
    /// @brief texture with shell
    sf::Texture shell;
    /// @brief texture with canvas frame
    sf::Texture frame;
    /// @brief font for text on canvas
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
        /// @brief array with canvas frames
        std::array<Object,4> frames;
        /// @brief pointer to player ship
        std::unique_ptr<PlayerShip> player;        
        /// @brief pointer to SFML window
        std::unique_ptr<sf::RenderWindow> window;
        /// @brief actual view grid
        sf::Vector2f grid; 
        /// @brief struct with game control items
        GameControl control;
        /// @brief struct with game configuration
        GameConfig config;
        /// @brief struct with game status
        GameStatus status;
        /// @brief struct with other canvas items, like score and player lives 
        GameMenuSprites menu_sprites;
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
        /// @brief setup all non moving canvas items 
        void setMenuSprites();
        /// @brief SFML event executor for windowEventHandler
        /// @param event reference to actual captured event
        void executeEvent(const sf::Event& event);
        /// @brief generate a new shell on the canvas
        /// @param rectangle reference to object rectangle from which shell be generated (shot animation)
        /// @param shell_type shell type (who shot this shell)
        void objectShot(const sf::FloatRect& rectangle, const ShellType shell_type);
};      

#endif //CANVAS_H
