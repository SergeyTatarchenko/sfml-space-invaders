/**
 * @file canvas.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#include <iostream>
#include <cstdlib>

#include "canvas.hpp"
#include "items.hpp"

#define MAX_FRAMERATE (unsigned int)(120)

//window title
static const sf::String title = "Space Invaders";
//default size for coordinates for main view
constexpr float default_start_x = 0.f;
constexpr float default_start_y = 0.f;
constexpr float default_x_size  = 1000.f;
constexpr float default_y_size  = 1000.f;
//border size around game field
constexpr float default_border_size = 50.f;
//game event tick
constexpr int game_event_tick_ms = 50;
//max 20 items per one row
constexpr float grid_row_step = default_x_size/20.f;
//invaders constants for grid and speed
constexpr int   invaders_in_row       = 18;
constexpr int   rows_with_invaders    = 2;
//speed setup
constexpr float default_invader_speed = (default_x_size + default_y_size)/(2.f * 10.f);
constexpr float default_ship_speed    = (default_x_size + default_y_size)/(2.f * 4.f);
constexpr float default_player_speed  = (default_x_size + default_y_size)/(2.f * 2.f);
constexpr float default_shell_speed   = (default_x_size + default_y_size)/(2.f * 2.f) ;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    using namespace std::chrono_literals;
    this->framerate = framerate;
    timer.step = 1ms*(1000/framerate);

    this->grid_x    = default_x_size;
    this->grid_y    = default_y_size;
    this->p_window  = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->player    = new PlayerShip(default_border_size,this->grid_x - default_border_size,default_player_speed,this->grid_x,this->grid_y);
    if(this->p_window->isOpen() == true)
    {
        this->game_in_progress.store(true,std::memory_order_relaxed);
        this->p_graphic_thread    = new std::thread(&Canvas::graphicThreadHandler,this);
        this->p_game_event_thread = new std::thread(&Canvas::gameEventGenerator,this);
        //disable window control (it will be controlled in p_graphic_thread)
        this->p_window->setActive(false); 
    }
}

Canvas::~Canvas()
{
    delete this->p_window;
    delete this->p_graphic_thread;
    delete this->p_game_event_thread;
}

void Canvas::windowEventHandler()
{    
    // we will exit this function only if window was requested to be closed
    while (this->p_window->isOpen())
    {
        sf::Event event;
        bool stat = true;
        while(stat == true)
        {
            stat = this->p_window->pollEvent(event);
            if(stat == true){this->eventExecutor(event);}
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(game_event_tick_ms));
    }
}

void Canvas::updateCanvas()
{
    //update enemies 
    for (Invader& enemy : this->enemies)
    {
        if(enemy.isVisible() == true){this->p_window->draw(enemy.getSprite());}
    }
    //update bullets
    for (Shell& shell : this->bullets)
    {
        if(shell.isVisible() == true){this->p_window->draw(shell.getSprite());}
    }
    //update enemy ships
    for (InvaderShip& ship : this->enemyShips)
    {
        if(ship.isVisible() == true){this->p_window->draw(ship.getSprite());}
    }
    //update player ship
    this->p_window->draw(this->player->getSprite());
}

void Canvas::updateItemsPosition()
{
    //update enemies 
    for (Invader& enemy : enemies)
    {
        if(enemy.isVisible() == true){enemy.moveAlongTrajectory(this->framerate);}
    }
    //update bullets
    for (Shell& shell : bullets)
    {
        if(shell.isVisible() == true){shell.moveAlongTrajectory(this->framerate);}
    }
    //update enemy ships
    for (InvaderShip& ship : enemyShips)
    {
        if(ship.isVisible() == true){ship.moveAlongTrajectory(this->framerate);}
    }
    //update player ship
    this->player->moveAlongTrajectory(this->framerate);
}

void Canvas::controlItemsPosition()
{
    //bullets control
    for (Shell& shell : bullets)
    {
        auto position = shell.getRectangle().getPosition();
        if((position.x > grid_x) || (position.x < default_start_x) ||
           (position.y > grid_y) || (position.y < default_start_y)
          )
        {
            shell.setInvisible();
        }
    }
}

void Canvas::updateFramerate(const unsigned int framerate)
{
    if(framerate <= MAX_FRAMERATE){this->framerate = framerate;}
    else{this->framerate = MAX_FRAMERATE;}
}

void Canvas::graphicThreadHandler()
{
    using namespace std::chrono_literals;
    timer.previous_time = std::chrono::system_clock::now();
    //obtain control on window
    if(this->p_window->setActive(true) == true)
    {
        // all graphic drawings are here
        while (this->game_in_progress.load(std::memory_order_relaxed) == true)
        {
            timer.actual_time  = std::chrono::system_clock::now();
            auto actual_period = timer.actual_time - timer.previous_time;
            timer.deviation = (std::chrono::duration_cast<std::chrono::milliseconds>(actual_period).count() * 1ms) - timer.step;
            sf::View view(sf::FloatRect(default_start_x, default_start_y, default_x_size, default_y_size));
            this->p_window->setView(view);
            this->p_window->clear(sf::Color::Black); 
            while(this->game_context_control.try_lock() == false){}
            this->updateCanvas();
            this->p_window->display();
            this->controlItemsPosition();
            this->updateItemsPosition();
            this->game_context_control.unlock();
            timer.previous_time = timer.actual_time;
            std::this_thread::sleep_for(timer.step - timer.deviation);
        }
        this->p_window->setActive(false);
    }
}

void Canvas::gameEventGenerator()
{


    static uint32_t counter = 0;   
    
    // we will exit this function only if window was requested to be closed
    while (this->game_in_progress.load(std::memory_order_relaxed) == true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(game_event_tick_ms));
        counter++;
        if((counter % 20) == 0)
        {
            counter = 0;
            while(this->game_context_control.try_lock() == false){}
            auto index = rand() % this->enemies.size();
            const auto rectangle = this->enemies[index].getRectangle();
            this->objectShot(rectangle,ShellType::ENEMY);
            this->game_context_control.unlock();
        }
        if(this->player->getShotRequest() == true)
        {
            this->player->setShotRequest(false);

            while(this->game_context_control.try_lock() == false){}
            const auto rectangle = this->player->getRectangle();
            this->objectShot(rectangle,ShellType::PLAYER);
            this->game_context_control.unlock();
        }
    }
}

void Canvas::eventExecutor(const sf::Event &event)
{
    static bool player_reload = false;
    static bool left_pressed  = false;
    static bool right_pressed = false;

    switch (event.type)
    {
        case sf::Event::Closed:
            // game stop, threads stop, window closed
            this->game_in_progress.store(false,std::memory_order_relaxed);
            this->p_graphic_thread->join();
            this->p_game_event_thread->join();
            this->p_window->setActive(true);
            this->p_window->close();
            break;
    
        case sf::Event::KeyPressed:
            // game controls
            switch(event.key.code)
            {
                case sf::Keyboard::Key::Left:
                    left_pressed = true;
                    if(right_pressed == false){this->player->setDirection(ItemDirection::LEFT);}
                    break;

                case sf::Keyboard::Key::Right:
                    right_pressed = true;
                    if(left_pressed == false){this->player->setDirection(ItemDirection::RIGHT);}
                    break;

                case sf::Keyboard::Key::Space:
                    if(player_reload == false)
                    {
                        this->player->setShotRequest(true);
                        player_reload = true;
                    }
                    break;
                
                default:
                    break;
            }
            break;
        
        case sf::Event::KeyReleased:
            switch(event.key.code)
            {
                case sf::Keyboard::Key::Left:
                    left_pressed = false;
                    break;

                case sf::Keyboard::Key::Right:
                    right_pressed = false;
                    break;

                case sf::Keyboard::Key::Space:
                    if(player_reload == true){player_reload = false;}
                    break;
                
                default:
                    break;
            }
            if((left_pressed == false) && (right_pressed == false))
            {
                this->player->setDirection(ItemDirection::NONE);
            }
            break;

    default:
        break;
    }
}

void Canvas::objectShot(const sf::FloatRect &rectangle, const ShellType shell_type)
{
    //we are going to shut from the middle of the object     
    float x = rectangle.getPosition().x + rectangle.width/2.0f;
    float y = rectangle.getPosition().y + rectangle.height/2.0f;
    //check if we have available shells in array(that was already created and executed)
    auto it = std::find_if(bullets.begin(),bullets.end(),[]( Shell& shell){return shell.isVisible() == false;} );
    if(it != bullets.end())
    {
        //use existed one
        it->setShellType(shell_type);
        it->setPosition(x,y);
        it->setVisible();
    }
    else
    {
        //create new one
        bullets.emplace_back(x,y,default_shell_speed,shell_type);
    }
}

void Canvas::spawnEnemies()
{
    //enemies
    Invader invader(0.f,0.f,default_invader_speed,true);
    const float init_x = 50.f;
    const float init_y = 100.f;
    
    while(this->game_context_control.try_lock() == false){}
    float offset_y = 0.f;
    for(auto j = 0; j < rows_with_invaders; j++)
    {
        float offset_x = 0.f;
        for(auto i = 0; i < invaders_in_row; i++)
        {
            invader.setPosition((init_x + offset_x),(init_y + offset_y));
            this->enemies.push_back(invader);
            offset_x += grid_row_step;
        }
        offset_y += grid_row_step;
    }
    // enemy ships
    InvaderShip ship(default_border_size,default_border_size,default_ship_speed,true,this->grid_x - default_border_size*2);
    this->enemyShips.push_back(ship);

    this->game_context_control.unlock();
}

