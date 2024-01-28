/**
 * @file canvas.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#include <cstdlib>
#include <cstring>

#include "canvas.hpp"
#include "items.hpp"

//framerate limit
constexpr int max_framerate = 120;
//window title
static const sf::String title = "Space Invaders";
//default size for coordinates for main view
constexpr float default_start_x = 0.f;
constexpr float default_start_y = 0.f;
constexpr float default_x_size  = 1000.f;
constexpr float default_y_size  = 1000.f;
//border size around game field
constexpr float default_border_size = 50.f;
//max 20 items per one row
constexpr float grid_row_step = default_x_size/20.f;
//invaders constants for grid and speed
constexpr int   invaders_in_row       = 18;
constexpr int   rows_with_invaders    = 2;
//speed setup
constexpr float default_invader_speed = 1.f;
constexpr float default_ship_speed    = 4.f;
constexpr float default_shell_speed   = 4.f;
constexpr float default_player_speed  = 5.f;
//limits for player movement
constexpr float bottom_left_x   = default_border_size;
constexpr float bottom_right_x  = default_x_size - default_border_size;
constexpr float bottom_left_y   = default_y_size - default_border_size;
constexpr float bottom_right_y  = default_y_size - default_border_size;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    grid.x    = default_x_size;
    grid.y    = default_y_size;
    window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), title));
    window->setActive(true);
    window->setFramerateLimit(framerate);
    player = std::unique_ptr<PlayerShip>(new PlayerShip(sf::Vector2f(bottom_left_x,bottom_left_y),grid, default_player_speed));
    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
    std::memset(&game_control,0,sizeof(game_control));
    game_config.framerate           = framerate;
    game_config.invader_shot_period = framerate;
}

void Canvas::gameTask()
{
    sf::Event event;
    while (window->isOpen())
    {
        generateGameEvent();
        while(window->pollEvent(event)){executeEvent(event);}
        sf::View view(sf::FloatRect(default_start_x, default_start_y, default_x_size, default_y_size));
        window->setView(view);
        window->clear(sf::Color::Black); 
        updateCanvas();
        window->display();
        updateItemsPosition();
        controlItemsPosition();
    }
}

void Canvas::updateCanvas()
{
    //update enemies 
    for (Invader& enemy : enemies)
    {
        if(enemy.isVisible() == true){window->draw(enemy.getSprite());}
    }
    //update enemy ships
    for (InvaderShip& ship : enemyShips)
    {
        if(ship.isVisible() == true){window->draw(ship.getSprite());}
    }    
    //update bullets
    for (Shell& shell : bullets)
    {
        if(shell.isVisible() == true){window->draw(shell.getSprite());}
    }
    //update player ship
    this->window->draw(this->player->getSprite());
}

void Canvas::updateItemsPosition()
{
    //update enemies
    for (Invader& enemy : enemies){enemy.updatePosition();}
    //update enemy ships
    for (InvaderShip& ship : enemyShips){ship.updatePosition();}
    //update bullets
    for (Shell& shell : bullets){shell.updatePosition();}
    //update player ship
    this->player->updatePosition();
}

void Canvas::controlItemsPosition()
{
    //bullets control
    for (Shell& shell : bullets)
    {
        auto position = shell.getRectangle().getPosition();
        if((position.x > grid.x) || (position.x < default_start_x) ||
           (position.y > grid.y) || (position.y < default_start_y)
          )
        {
            shell.setInvisible();
        }
    }
}

void Canvas::generateGameEvent()
{    
    game_control.event_counter++;
    
    if((game_control.event_counter % game_config.invader_shot_period) == 0)
    {
        game_control.event_counter = 0;
        auto index = rand() % this->enemies.size();
        const auto rectangle = this->enemies[index].getRectangle();
        this->objectShot(rectangle,ShellType::ENEMY);
    }
    if(this->player->getShotRequest() == true)
    {
        this->player->setShotRequest(false);
        const auto rectangle = this->player->getRectangle();
        this->objectShot(rectangle,ShellType::PLAYER);
    }
}

void Canvas::executeEvent(const sf::Event &event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            // game stop, window closed
            window->close();
            break;
    
        case sf::Event::KeyPressed:
            // game controls
            switch(event.key.code)
            {
                case sf::Keyboard::Key::Left:
                    game_control.left_pressed = true;
                    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
                    break;

                case sf::Keyboard::Key::Right:
                    game_control.right_pressed = true;
                    player->setMotionVector(sf::Vector2f(bottom_right_x,bottom_right_y));
                    break;

                case sf::Keyboard::Key::Space:
                    if(game_control.player_reload == false)
                    {
                        player->setShotRequest(true);
                        game_control.player_reload = true;
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
                    game_control.left_pressed = false;
                    break;

                case sf::Keyboard::Key::Right:
                    game_control.right_pressed = false;
                    break;

                case sf::Keyboard::Key::Space:
                    if(game_control.player_reload == true){game_control.player_reload = false;}
                    break;
                
                default:
                    break;
            }
            if((game_control.left_pressed == false) && (game_control.right_pressed == false))
            {
                player->setMotionVector(player->getRectangle().getPosition());
            }
            break;

    default:
        break;
    }
}

void Canvas::objectShot(const sf::FloatRect &rectangle, const ShellType shell_type)
{
    //we are going to shut from the middle of the object
    sf::Vector2f position;     
    position.x = rectangle.getPosition().x + rectangle.width/2.0f;
    position.y = rectangle.getPosition().y + rectangle.height/2.0f;
    //check if we have available shells in array(that was already created and executed)
    auto it = std::find_if(bullets.begin(),bullets.end(),[]( Shell& shell){return shell.isVisible() == false;} );
    if(it != bullets.end())
    {
        //use existed one
        it->setShellType(shell_type);
        it->setPosition(position);
        it->setVisible();
    }
    else
    {
        //create new one
        bullets.emplace_back(position,default_shell_speed,shell_type);
    }
}

void Canvas::spawnEnemies()
{
    //enemies
    Invader invader(sf::Vector2f(0.f,0.f),default_invader_speed,true);
    constexpr float init_x = default_border_size;
    constexpr float init_y = default_border_size * 2.f;
    
    float offset_y = 0.f;
    for(auto j = 0; j < rows_with_invaders; j++)
    {
        float offset_x = 0.f;
        for(auto i = 0; i < invaders_in_row; i++)
        {
            invader.setInitPosition(sf::Vector2(init_x + offset_x,init_y + offset_y));
            invader.setDefaultPosition();
            enemies.push_back(invader);
            offset_x += grid_row_step;
        }
        offset_y += grid_row_step;
    }
    // enemy ships
    InvaderShip ship(sf::Vector2(default_border_size,default_border_size),default_ship_speed,true,grid.x - default_border_size);
    this->enemyShips.push_back(ship);
}

