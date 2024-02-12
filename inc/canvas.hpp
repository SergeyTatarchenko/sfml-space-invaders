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

#include <cstdint>
#include <vector>
#include <array>
#include <memory>
#include <random>
#include <SFML/Audio.hpp>
#include "items.hpp"

enum class GameStatus
{
    NotStarted,
    Running,
    GameOver
};

struct GameControl
{
    /// @brief counter for invader shot event, event will be generated when this counter be equal to invader_shot_period
    std::uint32_t invader_shot_counter = 0;
    /// @brief counter for invader ship spawn event, event will be generated when this counter be equal to ship_spawn_period
    std::uint32_t ship_spawn_counter = 0;
    /// @brief counter for player reload event, event will be generated when this counter be equal to player_reload_period
    std::uint32_t player_reload_counter = 0;
    /// @brief actual number of invader on th canvas
    std::uint32_t invaders_left = 0;
    /// @brief flag that invader ship is on the canvas now
    bool invader_ship_spawned = false;
    /// @brief label for player reload logic, when shot is not performed
    bool player_reload = false;
    /// @brief left key is pressed
    bool left_pressed = false;
    /// @brief right key is pressed
    bool right_pressed = false;
};

struct GameConfig
{
    /// @brief shot period, compared with event_counter from GameControl
    std::uint32_t invader_shot_period = 0;
    /// @brief ship spawn period, compared with event_counter from GameControl
    std::uint32_t ship_spawn_period = 0;
    /// @brief player reload period in framerate ticks
    std::uint32_t player_reload_period = 0;
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

struct GameSounds
{
    // shot sound buffer
    sf::SoundBuffer shoot_sound_buffer;
    // shot sound interface
    sf::Sound shoot_sound;
    // invader killed sound buffer
    sf::SoundBuffer invader_killed_sound_buffer;
    // invader killed sound interface
    sf::Sound invader_killed_sound;
    // player killed sound buffer
    sf::SoundBuffer player_killed_sound_buffer;
    // player killed sound interface
    sf::Sound player_killed_sound;
    // ship sound buffer
    sf::SoundBuffer ship_sound_buffer;
    // ship sound interface
    sf::Sound ship_sound;
};

struct GameResources
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
    /// @brief struct with game sounds resources
    GameSounds sounds;
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
        /// @brief struct with resources for game objects 
        GameResources resources;
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
        /// @brief spawn invaders on the canvas
        void spawnInvaders();
        /// @brief spawn player obstacles on the canvas
        void spawnObstacles();
        /// @brief spawn invader ship on the canvas
        void spawnInvaderShip();
        /// @brief game event generator
        void generateGameEvent();
        /// @brief textures loading
        void loadResources();
        /// @brief loading sounds
        void setupSounds();
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
