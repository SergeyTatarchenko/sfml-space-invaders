/**
 * @file window.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "canvas.hpp"

//default size for coordinates for main view
static const float default_x_size = 1000.f;
static const float default_y_size = 1000.f;
static const sf::String title = "Space Invaders";

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    this->framerate = framerate;
    this->p_window  = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->p_thread  = new std::thread(&Canvas::threadHandler,this);
    //disable window control (it will be controlled in p_thread)
    this->p_window->setActive(false); 
}

Canvas::~Canvas()
{
    delete p_thread;
    delete p_window;
}

void Canvas::windowEventHandler()
{
    using namespace std::chrono_literals;
    
    // we will exit this function only if window was requested to be closed
    while (this->p_window->isOpen())
    {
        sf::Event event;
        bool stat = true;
        while(stat == true)
        {
            stat = this->p_window->pollEvent(event);
            if(stat == true)
            {
                this->eventExecutor(event);
            }
        }
        std::this_thread::sleep_for(50ms);
    }
    this->p_thread->join();
}

void Canvas::threadHandler()
{
    //obtain control on window
    if(!this->p_window->setActive(true)){return;}
    else
    {
        // all GUI drawings are here
        while (this->p_window->isOpen())
        {
            sf::View view(sf::FloatRect(0.f, 0.f, default_x_size, default_y_size));
            this->p_window->setView(view);
            this->p_window->clear(sf::Color::White); 
            this->p_window->display();
            unsigned int delay_per_frame = 1000/this->framerate;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_frame));
        }
    }
}

void Canvas::eventExecutor(const sf::Event &event)
{
    switch (event.type)
    {

    case sf::Event::Closed:
        this->p_window->close();
        break;
    
    default:
        break;
    }
}
