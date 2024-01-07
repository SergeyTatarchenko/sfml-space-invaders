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
        
        Canvas(const unsigned int width, unsigned int height, const unsigned int framerate);
        ~Canvas();
        void updateFramerate(const unsigned int framerate);
        void windowEventHandler();
        void spawnEnemies();
        
    private:
    
        std::vector<Invader> enemies;
        unsigned int framerate;
        sf::RenderWindow* p_window;
        std::thread* p_thread;
        void threadHandler();
        void updateCanvas();
        void updateItemsPosition();
        void eventExecutor(const sf::Event& event);

};

#endif //CANVAS_H
