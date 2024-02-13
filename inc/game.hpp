/**
 * @file game.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <vector>
#include <memory>
#include <random>
#include <SFML/Audio.hpp>
#include "items.hpp"

namespace si
{
    ////////////////////////GAME SETTINGS AND CONSTANTS/////////////////////////////
    constexpr int   frame_width        = 10;
    constexpr int   frame_length       = 50;
    constexpr float default_start_x    = 0.f;
    constexpr float default_start_y    = 0.f;
    constexpr float default_x_size     = 1000.f;
    constexpr float default_y_size     = 1000.f;
    //border size around game field
    constexpr float default_border_size = 50.f;
    //max 10 items per one row
    constexpr float grid_row_step = default_x_size/15.f;
    //speed setup (greed per second)
    constexpr float default_invader_speed = 30.f;
    constexpr float default_ship_speed    = 100.f;
    constexpr float default_shell_speed   = 200.f;
    constexpr float default_player_speed  = 400.f;
    //limits for player movement
    constexpr float bottom_left_x   = static_cast<float>(frame_width);
    constexpr float bottom_right_x  = default_x_size - static_cast<float>(frame_width);
    constexpr float bottom_left_y   = default_y_size - default_border_size;
    constexpr float bottom_right_y  = default_y_size - default_border_size;
    //game logic config
    constexpr int   invaders_in_row     = 10;
    constexpr int   rows_with_invaders  = 6; 
    constexpr int invader_shot_period_s = 1;
    constexpr int ship_spawn_period_s   = 15;
    constexpr int invader_reward        = 10;
    constexpr int invader_ship_reward   = 250;
    constexpr int default_num_of_lives  = 3;
    constexpr int max_num_of_lives      = 5;
    ////////////////////////////////////////////////////////////////////////////////
    
    enum class GameStatus
    {
        NotStarted,
        Running,
        GameOver,
        Closed
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

    struct GameElements
    {
        /// @brief actual game score
        int score;
        /// @brief actual number of player lives
        int player_lives;
    };

    struct GameSounds
    {
        // shot sound interface
        sf::Sound shoot_sound;
        // invader killed sound interface
        sf::Sound invader_killed_sound;
        // player killed sound interface
        sf::Sound player_killed_sound;
        // ship sound interface
        sf::Sound ship_sound;
    };

    class Game
    {
        public:
            Game(unsigned int framerate);
            /// @brief vector with invaders
            std::vector<Invader> enemies;
            /// @brief vector with shell instances
            std::vector<Shell> bullets;
            /// @brief vector with player obstacles from invaders
            std::vector<Obstacle> obstacles;
            /// @brief pointer to player ship
            std::unique_ptr<PlayerShip> player;        
            /// @brief pointer to invader ship
            std::unique_ptr<InvaderShip> invader_ship;
            /// @brief actual game status
            GameStatus status;
            /// @brief used game sounds
            GameSounds sounds;
            /// @brief struct with game elements
            GameElements elements;
            /// @brief main game loop
            void gameLoop();
            /// @brief calculate items speed based on actual framerate
            /// @param framerate expected framerate
            void calculateItemsSpeed(const unsigned int framerate);
            /// @brief SFML event executor for windowEventHandler
            /// @param event reference to actual captured event
            void executeEvent(const sf::Event& event);

        private:
            /// @brief struct with game control items
            GameControl control;
            /// @brief struct with game configuration
            GameConfig config;
            /// @brief random number generator instance
            std:: minstd_rand randomizer;
            /// @brief setup invader instances
            void setupInvaders();
            /// @brief setup player obstacle instances
            void setupObstacles();
            /// @brief spawn invaders on the canvas
            void spawnInvaders();
            /// @brief spawn player obstacles on the canvas
            void spawnObstacles();
            /// @brief update items on canvas according to their trajectory
            void updateItemsPosition();
            /// @brief control all items on canvas and remove them if they leave visible space
            void controlItemsPosition();
            /// @brief check for collision between sprites on the canvas
            void checkCollision();
            /// @brief game event generator
            void generateGameEvent();
            /// @brief handler for player hitting by invader event
            void handlePlayerHitting();
            /// @brief spawn invader ship on the canvas
            void spawnInvaderShip();
            /// @brief generate a new shell on the canvas
            /// @param rectangle reference to object rectangle from which shell be generated (shot animation)
            /// @param shell_type shell type (who shot this shell)
            void objectShot(const sf::FloatRect& rectangle, const ShellType shell_type);    
    };
}      

#endif //GAME_H
