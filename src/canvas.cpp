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
////////////////////////GAME SETTINGS AND CONSTANTS/////////////////////////////

constexpr int ultimate_answer = 42;//init for random generator 
//window title
static const sf::String title = "Space Invaders";
//game version
static const sf::String version = "0.01";
//sizes for main view coordinates
constexpr float default_start_x = 0.f;
constexpr float default_start_y = 0.f;
constexpr float default_x_size  = 1000.f;
constexpr float default_y_size  = 1000.f;
//sizes for white frames around the canvas
constexpr int   frame_width     = 10;
constexpr int   frame_length    = 50;
//border size around game field
constexpr float default_border_size = 50.f;
//max 10 items per one row
constexpr float grid_row_step = default_x_size/15.f;
//invaders constants for grid and speed
constexpr int   invaders_in_row       = 10;
constexpr int   rows_with_invaders    = 6;
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
//setup for game score text size
constexpr int font_size = 32;
//game logic config
constexpr int invader_shot_period_s = 1;
constexpr int ship_spawn_period_s   = 15;
constexpr int invader_reward        = 10;
constexpr int invader_ship_reward   = 250;
constexpr int default_num_of_lives  = 3;
constexpr int max_num_of_lives      = 5;

////////////////////////////////////////////////////////////////////////////////

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    //initial game setup
    loadTextures();
    std::memset(&control,0,sizeof(control));
    std::memset(&status,0,sizeof(status));
    calculateItemsSpeed(framerate);
    setMenuSprites();
    grid.x                      = default_x_size;
    grid.y                      = default_y_size;
    status.game_status          = GameStatus::NOT_STARTED;
    config.invader_shot_period  = framerate * invader_shot_period_s;
    config.ship_spawn_period    = framerate * ship_spawn_period_s;
    config.player_reload_period = framerate/4;
    status.player_lives         = default_num_of_lives;
    // main window setup
    window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), title));
    window->setActive(true);
    window->setFramerateLimit(framerate);
    //player ship setup
    player = std::unique_ptr<PlayerShip>(new PlayerShip(sf::Vector2f(bottom_left_x,bottom_left_y),config.player_speed));
    player->setInitPosition(sf::Vector2f(bottom_left_x,bottom_left_y));
    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
    player->setTexture(resource_manager.player);
    //invader ship setup
    invader_ship = std::unique_ptr<InvaderShip>(new InvaderShip(sf::Vector2f(default_border_size,default_border_size*2.f),config.enemy_ship_speed,true));
    invader_ship->setTexture(resource_manager.enemy_ship);
    invader_ship->setInvisible();
    //random generator used for enemy shot events
    randomizer.seed(ultimate_answer);
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
        while(window->pollEvent(event)){executeEvent(event);}
        sf::View view(sf::FloatRect(default_start_x, default_start_y, default_x_size, default_y_size));
        window->setView(view);
        window->clear(sf::Color::Black);
        switch(status.game_status)
        {
            case GameStatus::NOT_STARTED:
                drawWelcomeWindow();
                break;
            
            case GameStatus::RUNNING:
                updateCanvas();
                generateGameEvent();
                controlItemsPosition();
                checkCollision();
                updateItemsPosition();
                break;
            
            case GameStatus::GAME_OVER:
                drawGameOverScreen();
                break;
                
            default:
                break;
        }
        window->display();
    }
}

void Canvas::updateCanvas()
{
    //update score indicator
    window->draw(menu_sprites.score);
    //update lives indicator
    drawPlayerLives();
    //update menu frames
    for(Object & frame : menu_sprites.frames){window->draw(frame.getSprite());}
    //update enemies 
    for (Invader& enemy : enemies)
    {
        if(enemy.isVisible() == true){window->draw(enemy.getSprite());}
    }    
    //update bullets
    for (Shell& shell : bullets)
    {
        if(shell.isVisible() == true){window->draw(shell.getSprite());}
    }
    //update obstacles
    for (Obstacle& obstacle : obstacles)
    {
        if(obstacle.isVisible() == true){window->draw(obstacle.getSprite());}
    }
    //update enemy ship
    if(invader_ship->isVisible() == true){window->draw(invader_ship->getSprite());}
    //update player ship
    window->draw(player->getSprite());
}

void Canvas::updateItemsPosition()
{
    //update enemies
    for (Invader& enemy : enemies){enemy.updatePosition();}
    //update enemy ship
    invader_ship->updatePosition();
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
        if(shell.isVisible() == true)
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
    //enemy ship control
    if(invader_ship->isVisible() == true)
    {
        auto position = invader_ship->getRectangle().getPosition();
        if((position.x > grid.x) || (position.x < default_start_x) ||
        (position.y > grid.y) || (position.y < default_start_y)
        )
        {
            invader_ship->setInvisible();
            control.invader_ship_spawned = false;
        }
    }
    //spawn invaders again 
    if(control.invaders_left == 0)
    {
        //add logic for delay and music play
        spawnInvaders();
    }
}

void Canvas::generateGameEvent()
{
    //every tick for invaders
    control.invader_shot_counter++;
    //only if ship not spawned already
    if(control.invader_ship_spawned == false){control.ship_spawn_counter++;}
    //only if player shot, reload delay
    if(control.player_reload == true){control.player_reload_counter++;}
    //random enemy shot every second
    if((control.invader_shot_counter % config.invader_shot_period) == 0)
    {
        auto index = randomizer() % enemies.size();
        if(enemies[index].isVisible() == true)
        {
            const auto rectangle = enemies[index].getRectangle();
            objectShot(rectangle,ShellType::ENEMY);
        }
        
    }
    //generate invader ship spawn event
    if(((control.ship_spawn_counter % config.ship_spawn_period) == 0) &&
       (control.invader_ship_spawned == false))
    {
        spawnInvaderShip();
        control.ship_spawn_counter = 0;
        control.invader_ship_spawned = true;
    }
    //player shot handle 
    if(player->getShotRequest() == true)
    {
        player->setShotRequest(false);
        const auto rectangle = this->player->getRectangle();
        objectShot(rectangle,ShellType::PLAYER);
    }
    //player reload handle
    if(((control.player_reload_counter % config.player_reload_period) == 0) &&
       (control.player_reload == true))
    {
        control.player_reload = false;
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

    if(enemies.size() > 0)
    {
        for (Invader& enemy : enemies)
        {
            enemy.revertPosition();
            enemy.setVisible();
        }
    }
    else
    {
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
    control.invaders_left = enemies.size();
}

void Canvas::spawnObstacles()
{
    // 4 structs with obstacles, 10*5 obstacles in struct , 2 rows, initial start 120, 900
    constexpr int obstacles_in_row      = 10;
    constexpr int struct_with_obstacles = 4;
    constexpr float rows_with_obstacles = 5;
    constexpr float init_x              = 120.f;
    constexpr float init_y              = 900.f;

    if(obstacles.size() > 0)
    {
        for(Obstacle& obstacle : obstacles)
        {
            obstacle.setVisible();
        }
    }
    else
    {
        sf::Vector2f init(init_x,init_y);
        Obstacle obstacle(init);
        sf::FloatRect rectangle = obstacle.getRectangle();
        obstacle.setTexture(resource_manager.obstacle);
        for(auto i = 0; i < rows_with_obstacles; i++)
        {
            for(auto j = 0; j < struct_with_obstacles; j++)
            {
                for(auto k = 0; k < obstacles_in_row; k++)
                {
                    obstacle.setPosition(init);
                    obstacles.push_back(obstacle);
                    init.x += rectangle.width;
                }
                init.x += 120.f;
            }
            init.x  = init_x;
            init.y -= rectangle.height;
        } 
    }
}

void Canvas::spawnInvaderShip()
{
    invader_ship->setDefaultPosition();
    invader_ship->setVisible();
}

void Canvas::executeEvent(const sf::Event &event)
{
    sf::IntRect player_size = player->getSprite().getTextureRect();
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
                    player->setMotionVector(sf::Vector2f(bottom_right_x - static_cast<float>(player_size.width),bottom_right_y));
                    break;

                case sf::Keyboard::Key::Space:
                    switch(status.game_status)
                    {
                        case GameStatus::NOT_STARTED:
                            status.player_lives = default_num_of_lives;
                            spawnInvaders();
                            spawnObstacles();
                            status.game_status = GameStatus::RUNNING;    
                            break;
                        
                        case GameStatus::RUNNING:
                            if(control.player_reload == false)
                            {
                                player->setShotRequest(true);
                                control.player_reload = true;
                            }
                            break;
                        
                        case GameStatus::GAME_OVER:
                            status.game_status = GameStatus::NOT_STARTED;
                            break;
                        default:
                            break;
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
        if((shell.getShellType() == ShellType::ENEMY) && (shell.isVisible() == true))
        {
            //collision between enemy shells and player ship
            if(player->getRectangle().intersects(shell.getRectangle()))
            {
                handlePlayerHitting();
            }
        }

        if((shell.getShellType() == ShellType::PLAYER) && (shell.isVisible() == true))
        {
            //collision between player shells and invaders
            for (Invader& enemy : enemies)
            {

                if((enemy.isVisible() == true) && (shell.getRectangle().intersects(enemy.getRectangle()) == true))
                {
                    shell.setInvisible();
                    enemy.setInvisible();
                    control.invaders_left--;
                    status.score += invader_reward;
                }    

            }
            //collision between player shells and invader ship
            if((invader_ship->isVisible() == true) && (shell.getRectangle().intersects(invader_ship->getRectangle()) == true))
            {
                shell.setInvisible();
                invader_ship->setInvisible();
                status.score += invader_ship_reward;
                control.invader_ship_spawned = false;
            }
        }
        //collision between shells and player obstacles
        for (Obstacle& obstacle : obstacles)
        {
            if((obstacle.isVisible() == true)&& (shell.getRectangle().intersects(obstacle.getRectangle()) == true))
            {
                if(shell.getShellType() == ShellType::ENEMY)
                {
                    obstacle.setInvisible();
                    shell.setInvisible();
                }
                else if(shell.getShellType() == ShellType::PLAYER)
                {
                    shell.setInvisible();
                }
            }
        }
    }
}

void Canvas::setMenuSprites()
{
    //setup game score item
    menu_sprites.score.setFont(resource_manager.game_font);
    menu_sprites.score.setCharacterSize(font_size);
    menu_sprites.score.setPosition(sf::Vector2f(static_cast<float>(frame_length),static_cast<float>(frame_width)));
    //player lives indicator
    menu_sprites.live.setTexture(resource_manager.player);
    //setup canvas frames
    //up 1
    menu_sprites.frames[0].setSpriteRectangle(sf::IntRect(0,0,default_x_size,frame_width));
    menu_sprites.frames[0].setTexture(resource_manager.frame);
    menu_sprites.frames[0].setSpriteColor(sf::Color::White);
    menu_sprites.frames[0].setPosition(sf::Vector2f(default_start_x,default_start_y));
    //right
    menu_sprites.frames[1].setSpriteRectangle(sf::IntRect(0,0,frame_width,default_y_size));
    menu_sprites.frames[1].setTexture(resource_manager.frame);
    menu_sprites.frames[1].setSpriteColor(sf::Color::White);
    menu_sprites.frames[1].setPosition(sf::Vector2f(default_x_size - static_cast<float>(frame_width),default_start_y));
    //up 2
    menu_sprites.frames[2].setSpriteRectangle(sf::IntRect(0,0,default_x_size,frame_width));
    menu_sprites.frames[2].setTexture(resource_manager.frame);
    menu_sprites.frames[2].setSpriteColor(sf::Color::White);
    menu_sprites.frames[2].setPosition(sf::Vector2f(default_start_x,static_cast<float>(frame_length)));
    //left
    menu_sprites.frames[3].setSpriteRectangle(sf::IntRect(0,0,frame_width,default_y_size));
    menu_sprites.frames[3].setTexture(resource_manager.frame);
    menu_sprites.frames[3].setSpriteColor(sf::Color::White);
    menu_sprites.frames[3].setPosition(sf::Vector2f(default_start_x,default_start_y));
    //down
    menu_sprites.frames[4].setSpriteRectangle(sf::IntRect(0,0,default_x_size,frame_width));
    menu_sprites.frames[4].setTexture(resource_manager.frame);
    menu_sprites.frames[4].setSpriteColor(sf::Color::White);
    menu_sprites.frames[4].setPosition(sf::Vector2f(default_start_x,default_y_size - static_cast<float>(frame_width)));
}

void Canvas::drawPlayerLives()
{
    //small border to prevent sprites from sticking together
    constexpr float border = 10.f;
    sf::IntRect texture_size =  menu_sprites.live.getTextureRect();
    //initial offset, lives will be drawn from right to left
    float offset = default_x_size - frame_width - static_cast<float>(texture_size.width) - border;
    for(auto i = 0; i < status.player_lives; i++)
    {
        menu_sprites.live.setPosition(sf::Vector2f(offset,static_cast<float>(frame_width)));
        window->draw(menu_sprites.live);
        offset -= static_cast<float>(texture_size.width) + border;
    }
}

void Canvas::drawWelcomeWindow()
{
    sf::Text welcome_text;
    sf::Vector2f position(default_border_size,default_border_size);
    welcome_text.setFont(resource_manager.game_font);
    welcome_text.setCharacterSize(font_size);

    welcome_text.setString(title + " version : " + version);
    welcome_text.setPosition(position);
    window->draw(welcome_text);
    position.y += default_border_size;

    welcome_text.setString("Controls:");
    welcome_text.setPosition(position);
    window->draw(welcome_text);
    position.y += default_border_size;

    welcome_text.setString("Shot - Space Bar");
    welcome_text.setPosition(position);
    window->draw(welcome_text);
    position.y += default_border_size;

    welcome_text.setString("Movement - arrows");
    welcome_text.setPosition(position);
    window->draw(welcome_text);
    position.y += default_border_size;
    
    welcome_text.setString("Press Space key to start...");
    welcome_text.setPosition(position);
    window->draw(welcome_text);
}

void Canvas::drawGameOverScreen()
{
    sf::Text text;
    sf::Vector2f position(default_border_size,default_border_size);
    text.setFont(resource_manager.game_font);
    text.setCharacterSize(font_size);

    text.setString("GAME OVER");
    text.setPosition(position);
    window->draw(text);
    position.y += default_border_size;

    text.setString("Your score : " + std::to_string(status.score));
    text.setPosition(position);
    window->draw(text);
    position.y += default_border_size;

    text.setString("Press Space key to restart the game");
    text.setPosition(position);
    window->draw(text);
}

void Canvas::handlePlayerHitting()
{
    //remove all shells from canvas
    for (Shell& shell : bullets){shell.setInvisible();}
    if(status.player_lives > 0)
    {
        //decrease player lives counter
        status.player_lives--;
        //move player to default position
        player->setDefaultPosition();
        player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
        control.invader_shot_counter = 0;
    }
    else{status.game_status = GameStatus::GAME_OVER;}
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
    //shoot sound
    /*
    if(!resource_manager.shot_sound.loadFromFile("rc/sounds/shoot.waw"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    */
    resource_manager.player.setSmooth(true);
    resource_manager.enemy_ship.setSmooth(true);
    resource_manager.enemy_type_1.setSmooth(true);
    resource_manager.enemy_type_2.setSmooth(true);
    resource_manager.enemy_type_3.setSmooth(true);
}