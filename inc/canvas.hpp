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

enum class GameStatus
{
    NOT_STARTED,
    RUNNING,
    GAME_OVER
};

struct GameControl
{
    uint32_t invader_shot_counter;
    uint32_t ship_spawn_counter;
    bool invader_ship_spawned;
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
    /// @brief ship spawn period, compared with event_counter from GameControl
    unsigned int ship_spawn_period;
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
    /// @brief text with initial text
    sf::Text start_text;
    /// @brief sprite with player live
    sf::Sprite live;
    /// @brief array with canvas frames
    std::array<Object,5> frames; 
};

struct GameElements
{
    /// @brief actual game score
    int score;
    /// @brief actual number of player lives
    int player_lives;
    /// @brief flag if game was started by player
    GameStatus game_status;
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
    /// @brief texture for obstacles
    sf::Texture obstacle;
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
        /// @brief game main function
        void gameTask();

    private:
        /// @brief vector with invaders
        std::vector<Invader> enemies;
        /// @brief vector with shell instances
        std::vector<Shell> bullets;
        /// @brief vector with player obstacles from invaders
        std::vector<Obstacle> obstacles;
        /// @brief pointer to player ship
        std::unique_ptr<PlayerShip> player;        
        /// @brief pointer to SFML window
        std::unique_ptr<sf::RenderWindow> window;
        /// @brief pointer to invader ship
        std::unique_ptr<InvaderShip> invader_ship;
        /// @brief actual view grid
        sf::Vector2f grid; 
        /// @brief struct with game control items
        GameControl control;
        /// @brief struct with game configuration
        GameConfig config;
        /// @brief struct with game status
        GameElements status;
        /// @brief struct with other canvas items, like score and player lives 
        GameMenuSprites menu_sprites;
        /// @brief random number generator instance
        std:: minstd_rand randomizer;
        /// @brief struct with textures for game objects 
        ResourceManager resource_manager;
        /// @brief spawn invaders on the canvas
        void spawnInvaders();
        /// @brief spawn player obstacles on the canvas
        void spawnObstacles();
        /// @brief spawn invader ship on the canvas
        void spawnInvaderShip();
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
        /// @brief draw actual number of player lives
        void drawPlayerLives();
        /// @brief draw window with welcome and press and key screen
        void drawWelcomeWindow();
        /// @brief draw window with game over and final score
        void drawGameOverScreen();
        /// @brief handler for player hitting by invader event
        void handlePlayerHitting();
        /// @brief SFML event executor for windowEventHandler
        /// @param event reference to actual captured event
        void executeEvent(const sf::Event& event);
        /// @brief generate a new shell on the canvas
        /// @param rectangle reference to object rectangle from which shell be generated (shot animation)
        /// @param shell_type shell type (who shot this shell)
        void objectShot(const sf::FloatRect& rectangle, const ShellType shell_type);
};      

#endif //CANVAS_H
