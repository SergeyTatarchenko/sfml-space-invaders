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
#include <cstring>
#include "canvas.hpp"
#include "items.hpp"

//framerate limit
constexpr int max_framerate = 120;
//init for random generator 
constexpr int ultimate_answer = 42;
//window title
static const sf::String title = "Space Invaders";
//default size for coordinates for main view
constexpr float default_start_x = 0.f;
constexpr float default_start_y = 0.f;
constexpr float default_x_size  = 1000.f;
constexpr float default_y_size  = 1000.f;
//border size around game field
constexpr float default_border_size = 50.f;
//max 10 items per one row
constexpr float grid_row_step = default_x_size/15.f;
//invaders constants for grid and speed
constexpr int   invaders_in_row       = 13;
constexpr int   rows_with_invaders    = 6;
//speed setup (greed per second)
constexpr float default_invader_speed = 30.f;
constexpr float default_ship_speed    = 100.f;
constexpr float default_shell_speed   = 200.f;
constexpr float default_player_speed  = 400.f;
//limits for player movement
constexpr float bottom_left_x   = default_border_size;
constexpr float bottom_right_x  = default_x_size - default_border_size;
constexpr float bottom_left_y   = default_y_size - default_border_size;
constexpr float bottom_right_y  = default_y_size - default_border_size;
//setup for game score
constexpr int font_size      = 32;
constexpr int invader_reward = 10;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    //random generator used for enemy shot events
    randomizer.seed(ultimate_answer);
    grid.x = default_x_size;
    grid.y = default_y_size;
    loadTextures();
    calculateItemsSpeed(framerate);
    window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), title));
    window->setActive(true);
    window->setFramerateLimit(framerate);
    player = std::unique_ptr<PlayerShip>(new PlayerShip(sf::Vector2f(bottom_left_x,bottom_left_y),grid,config.player_speed));
    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
    player->setTexture(resource_manager.player);
    std::memset(&control,0,sizeof(control));
    std::memset(&status,0,sizeof(status));
    //shot every second
    config.invader_shot_period = framerate;
    //setup game score sprite
    menu_sprites.score.setFont(resource_manager.game_font);
    menu_sprites.score.setCharacterSize(font_size);
    menu_sprites.score.setPosition(sf::Vector2f(default_border_size,0.f));
}

void Canvas::calculateItemsSpeed(const unsigned int framerate)
{
    if(framerate != 0)
    {
        config.enemy_ship_speed = default_ship_speed/framerate;
        config.invader_speed    = default_invader_speed/framerate;
        config.player_speed     = default_player_speed/framerate;
        config.shell_speed      = default_shell_speed/framerate;
    }
    else{std::memset(&config,0,sizeof(config));}
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
        controlItemsPosition();
        checkCollision();
        updateItemsPosition();
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
    window->draw(player->getSprite());
    //update score
    window->draw(menu_sprites.score);
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
    player->updatePosition();
    //update game score
    menu_sprites.score.setString("SCORE: " + std::to_string(status.score));
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
    control.event_counter++;
    //random enemy shot every second
    if((control.event_counter % config.invader_shot_period) == 0)
    {
        control.event_counter = 0;
        auto index = randomizer() % enemies.size();
        if(enemies[index].isVisible() == true)
        {
            const auto rectangle = enemies[index].getRectangle();
            objectShot(rectangle,ShellType::ENEMY);
        }
        
    }
    //player shot handle 
    if(player->getShotRequest() == true)
    {
        player->setShotRequest(false);
        const auto rectangle = this->player->getRectangle();
        objectShot(rectangle,ShellType::PLAYER);
    }
}

void Canvas::spawnInvaders()
{
    constexpr float init_x = default_border_size;
    constexpr float init_y = default_border_size * 4.f;
    
    auto set_texture = [this](Invader &invader, int index)
    {
        switch (index)
        {
            case 1:
            case 4:
                invader.setTexture(resource_manager.enemy_type_2);
                break;
            case 2:
            case 5:
                invader.setTexture(resource_manager.enemy_type_3);
                break;
            case 0:
            case 3:
            default:
                invader.setTexture(resource_manager.enemy_type_1);
                break;
        }
    };

    Invader invader(sf::Vector2f(0.f,0.f),config.invader_speed,true);

    float offset_y = 0.f;
    for(auto j = 0; j < rows_with_invaders; j++)
    {
        float offset_x = 0.f;
        for(auto i = 0; i < invaders_in_row; i++)
        {
            invader.setInitPosition(sf::Vector2(init_x + offset_x,init_y + offset_y));
            invader.setDefaultPosition();
            set_texture(invader,j);
            enemies.push_back(invader);
            offset_x += grid_row_step;
        }
        offset_y += grid_row_step;
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
                    control.left_pressed = true;
                    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
                    break;

                case sf::Keyboard::Key::Right:
                    control.right_pressed = true;
                    player->setMotionVector(sf::Vector2f(bottom_right_x,bottom_right_y));
                    break;

                case sf::Keyboard::Key::Space:
                    if(control.player_reload == false)
                    {
                        player->setShotRequest(true);
                        control.player_reload = true;
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
                    control.left_pressed = false;
                    break;

                case sf::Keyboard::Key::Right:
                    control.right_pressed = false;
                    break;

                case sf::Keyboard::Key::Space:
                    if(control.player_reload == true){control.player_reload = false;}
                    break;
                
                default:
                    break;
            }
            if((control.left_pressed == false) && (control.right_pressed == false))
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
        Shell shell(position,config.shell_speed,shell_type);
        shell.setTexture(resource_manager.shell);
        shell.setSpriteColor(sf::Color(40, 236, 250));
        bullets.push_back(shell);
    }
}

void Canvas::checkCollision()
{
    
    for (Shell& shell : bullets)
    {
        //collision between enemy shells and player ship
        if((shell.getShellType() == ShellType::ENEMY) && (shell.isVisible() == true))
        {

            if(player->getRectangle().intersects(shell.getRectangle()))
            {
                //logic for player hit here
            }
        }
        if((shell.getShellType() == ShellType::PLAYER) && (shell.isVisible() == true))
        {
            for (Invader& enemy : enemies)
            {

                if( (enemy.isVisible() == true) && (shell.getRectangle().intersects(enemy.getRectangle()) == true))
                {
                    //invader hit, make it invisible, make shot invisible
                    shell.setInvisible();
                    enemy.setInvisible();
                    status.score += invader_reward;
                }    
            }
        }
    }
}

void Canvas::spawnEnemies()
{
    spawnInvaders();
    // enemy ships
    InvaderShip ship(sf::Vector2(default_border_size,default_border_size*2.f),config.enemy_ship_speed,true,grid.x - default_border_size);
    ship.setTexture(resource_manager.enemy_ship);
    enemyShips.push_back(ship);
}

void Canvas::loadTextures()
{
    //textures
    if(!resource_manager.enemy_type_1.loadFromFile("rc/textures/green.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resource_manager.enemy_type_2.loadFromFile("rc/textures/red.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resource_manager.enemy_type_3.loadFromFile("rc/textures/yellow.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resource_manager.player.loadFromFile("rc/textures/player.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resource_manager.enemy_ship.loadFromFile("rc/textures/extra.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //font
    if(!resource_manager.game_font.loadFromFile("rc/fonts/SpaceMission.ttf"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    resource_manager.player.setSmooth(true);
    resource_manager.enemy_ship.setSmooth(true);
    resource_manager.enemy_type_1.setSmooth(true);
    resource_manager.enemy_type_2.setSmooth(true);
    resource_manager.enemy_type_3.setSmooth(true);
}