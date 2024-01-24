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

#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

#include <SFML/Graphics.hpp>
#include "items.hpp"

struct Timer
{
    std::chrono::_V2::system_clock::time_point actual_time;
    std::chrono::_V2::system_clock::time_point previous_time;
    std::chrono::milliseconds step;
    std::chrono::milliseconds deviation;
};

class Canvas
{
    public:
        /// @brief default constructor
        /// @param width canvas initial width in pixels
        /// @param height canvas initial height in pixels
        /// @param framerate canvas initial framerate
        Canvas(const unsigned int width, unsigned int height, const unsigned int framerate);
        /// @brief default destructor
        ~Canvas();
        /// @brief update canvas framerate
        /// @param framerate new framerate
        void updateFramerate(const unsigned int framerate);
        /// @brief handler for SFML library events, must ne called in a loop
        void windowEventHandler();
        /// @brief spawn enemies on canvas
        void spawnEnemies();
        
    private:
        Timer timer;
        /// @brief vector with invader instances
        std::vector<Invader> enemies;
        std::vector<InvaderShip> enemyShips;        
        PlayerShip* player;
        /// @brief vector with shell instances
        std::vector<Shell> bullets;
        /// @brief actual canvas framerate
        unsigned int framerate;
        /// @brief actual grid for x
        float grid_x;
        /// @brief actual grid for y
        float grid_y;
        /// @brief pointer to SFML window
        sf::RenderWindow* p_window;
        /// @brief pointer to thread with all graphic management
        std::thread* p_graphic_thread;
        /// @brief pointer to thread with internal game events
        std::thread* p_game_event_thread;
        /// @brief  struct with internal game events
        std::atomic<bool> game_in_progress {false};
        /// @brief mutex for proper resource sharing between threads
        std::mutex game_context_control;
        /// @brief handler for p_graphic_thread 
        void graphicThreadHandler();
        /// @brief handler for p_game_event_thread
        void gameEventGenerator();
        /// @brief render items on canvas according to their actual state
        void updateCanvas();
        /// @brief update items on canvas according to their trajectory
        void updateItemsPosition();
        /// @brief control all items on canvas and remove them if they leave visible space
        void controlItemsPosition();
        /// @brief SFML event executor for windowEventHandler
        /// @param event reference to actual captured event
        void eventExecutor(const sf::Event& event);
        void objectShot(const sf::FloatRect& rectangle, const ShellType shell_type);
};      

#endif //CANVAS_H
