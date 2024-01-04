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
#include <SFML/Graphics.hpp>

class Canvas
{
    public:
        
        Canvas(const unsigned int width, unsigned int height, const unsigned int framerate);
        ~Canvas();
        void windowEventHandler();

    private:

        unsigned int framerate;
        sf::RenderWindow* p_window;
        std::thread* p_thread;
        void threadHandler();
        void eventExecutor(const sf::Event& event);

};

#endif //CANVAS_H
