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
#include <SFML/Graphics.hpp>
#include "items.hpp"

#define MAX_FRAMERATE (unsigned int)(120)

class Canvas
{
    public:
        /// @brief default constructor
        /// @param width canvas initial width in pixels
        /// @param height canvas initial height in pixels
        /// @param framerate canvas initial framerate
        Canvas(const unsigned int width, unsigned int height, const unsigned int framerate);
        /// @brief default desctuctor
        ~Canvas();
        /// @brief update canvas framerate
        /// @param framerate new framerate
        void updateFramerate(const unsigned int framerate);
        /// @brief handler for SFML library events, must ne called in a loop
        void windowEventHandler();
        /// @brief spawn enemies on canvas
        void spawnEnemies();
        
    private:
        /// @brief vector with invader instances
        std::vector<Invader> enemies;
        /// @brief actual canvas framerate
        unsigned int framerate;
        /// @brief pointer to SFML window
        sf::RenderWindow* p_window;
        /// @brief pointer to thread with all graphic management
        std::thread* p_graphic_thread;
        /// @brief pointer to thread with internal game events
        std::thread* p_game_event_thread;
        /// @brief handler for p_graphic_thread 
        void graphicThreadHandler();
        /// @brief handler for p_game_event_thread
        void gameEventHandler();
        /// @brief render items on canvas according to their actual state
        void updateCanvas();
        /// @brief update items on canvas according to their trajektory
        void updateItemsPosition();
        /// @brief SFML event executor for windowEventHandler
        /// @param event reference to actual captured event
        void eventExecutor(const sf::Event& event);
};

#endif //CANVAS_H
