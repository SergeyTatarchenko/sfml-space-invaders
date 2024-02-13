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

#include <array>
#include "game.hpp"

//canvas settings
constexpr          int num_of_frames    = 5;
constexpr          int font_size        = 32;
constexpr unsigned int canvas_width     = 500;
constexpr unsigned int canvas_height    = 500;

struct GameMenuSprites 
{
    /// @brief sprite with game score
    sf::Text score;
    /// @brief text with initial text
    sf::Text start_text;
    /// @brief sprite with player live
    sf::Sprite live;
    /// @brief array with canvas frames
    std::array<Object,num_of_frames> frames; 
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
    // shot sound buffer
    sf::SoundBuffer shoot_sound_buffer;
    // invader killed sound buffer
    sf::SoundBuffer invader_killed_sound_buffer;
    // player killed sound buffer
    sf::SoundBuffer player_killed_sound_buffer;
    // ship sound buffer
    sf::SoundBuffer ship_sound_buffer;
};

class Canvas
{
    public:
        /// @brief default constructor
        /// @param framerate canvas initial framerate
        Canvas( const unsigned int framerate);
        /// @brief game main function
        void runEventLoop();

    private:     
        /// @brief pointer to SFML window
        sf::RenderWindow window;
        /// @brief struct with resources for game objects 
        GameResources resources;
        /// @brief struct with other canvas items, like score and player lives 
        GameMenuSprites menu_sprites;
        /// @brief main game class
        si::Game game; 
        /// @brief resources loading from external files
        void loadResources();
        /// @brief setup game sounds
        void setupSounds();
        /// @brief setup items textures
        void setupTextures();
        /// @brief setup all non moving canvas items 
        void setupMenu();
        /// @brief render items on canvas according to their actual state
        void updateCanvas();
        /// @brief draw actual number of player lives
        void drawPlayerLives();
        /// @brief draw window with welcome and press and key screen
        void drawWelcomeWindow();
        /// @brief draw window with game over and final score
        void drawGameOverScreen();
};      

#endif //CANVAS_H
