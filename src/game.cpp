/**
 * @file game.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */
#include <ctime>
#include <algorithm>
#include <iostream>
#include "game.hpp"

using namespace si;

Game::Game(unsigned int framerate)
{
    calculateItemsSpeed(framerate);
    config.invader_shot_period  = framerate * invader_shot_period_s;
    config.ship_spawn_period    = framerate * ship_spawn_period_s;
    config.player_reload_period = framerate/4;
    status                      = GameStatus::NotStarted;
    setupInvaders();
    setupObstacles();
    player       = std::make_unique<PlayerShip>(PlayerShip(sf::Vector2f(bottom_left_x,bottom_left_y),config.player_speed));
    invader_ship = std::make_unique<InvaderShip>(InvaderShip(sf::Vector2f(default_border_size,default_border_size*2.f),config.enemy_ship_speed,false));
    player->setInitPosition(sf::Vector2f(bottom_left_x,bottom_left_y));
    player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
    //random generator used for enemy shot events
    randomizer.seed(std::time(nullptr));
    //create one invisible shell in shell vector to give possibility to setup texture and sprite
    Shell shell(sf::Vector2f(0.f,0.f),config.shell_speed,ShellType::Enemy);
    shell.setVisibility(false);
    bullets.push_back(shell);
}

void Game::gameLoop()
{
    generateGameEvent();
    controlItemsPosition();
    checkCollision();
    updateItemsPosition();
}

void Game::calculateItemsSpeed(const unsigned int framerate)
{
    if(framerate != 0)
    {
        config.enemy_ship_speed = default_ship_speed/framerate;
        config.invader_speed    = default_invader_speed/framerate;
        config.player_speed     = default_player_speed/framerate;
        config.shell_speed      = default_shell_speed/framerate;
    }
    else{config = GameConfig();}
}

void si::Game::gameRestart()
{
    elements = GameElements();
    spawnInvaders();
    spawnObstacles();
    status = GameStatus::Running;
}

void Game::setupInvaders()
{
    constexpr float init_x = default_border_size;
    constexpr float init_y = default_border_size * 4.f;
    
    if(enemies.empty())
    {
        Invader invader(sf::Vector2f(0.f,0.f),config.invader_speed,false);
        float offset_y = 0.f;
        for(auto j = 0; j < rows_with_invaders; ++j)
        {
            float offset_x = 0.f;
            for(auto i = 0; i < invaders_in_row; ++i)
            {
                invader.setInitPosition(sf::Vector2(init_x + offset_x,init_y + offset_y));
                invader.setDefaultPosition();
                enemies.push_back(invader);
                offset_x += grid_row_step;
            }
            offset_y += grid_row_step;
        }
        control.invaders_left = enemies.size();
    }
}

void Game::setupObstacles()
{
    // 4 structs with obstacles, 10*5 obstacles in struct , 2 rows, initial start 120, 900
    constexpr int obstacles_in_row      = 10;
    constexpr int struct_with_obstacles = 4;
    constexpr float rows_with_obstacles = 5;
    constexpr float init_x              = 120.f;
    constexpr float init_y              = 900.f;

    if(obstacles.empty())
    {
        sf::Vector2f init(init_x,init_y);
        Obstacle obstacle(init);
        sf::FloatRect rectangle = obstacle.getRectangle();
        for(auto i = 0; i < rows_with_obstacles; ++i)
        {
            for(auto j = 0; j < struct_with_obstacles; ++j)
            {
                for(auto k = 0; k < obstacles_in_row; ++k)
                {
                    obstacle.setPosition(init);
                    obstacles.push_back(obstacle);
                    init.x += rectangle.width;
                }
                init.x += init_x;
            }
            init.x  = init_x;
            init.y -= rectangle.height;
        } 
    }
}

void Game::spawnInvaders()
{
    if(!enemies.empty())
    {
        for (Invader& enemy : enemies)
        {
            enemy.revertPosition();
            enemy.setVisibility(true);
        }
        control.invaders_left = enemies.size();
    }
}

void Game::spawnObstacles()
{
    if(!obstacles.empty())
    {
        for(Obstacle& obstacle : obstacles)
        {
            obstacle.setVisibility(true);
        }
    }
}

void Game::updateItemsPosition()
{
    //update enemies
    for (Invader& enemy : enemies){enemy.updatePosition();}
    //update enemy ship
    invader_ship->updatePosition();
    //update bullets
    for (Shell& shell : bullets){shell.updatePosition();}
    //update player ship
    player->updatePosition();    
}

void Game::generateGameEvent()
{
    std::cout<<"invaders left : "<<control.invaders_left<<"\n";
    //every tick for invaders shot event
    ++control.invader_shot_counter;
    //only if ship not spawned already
    if(!control.invader_ship_spawned){++control.ship_spawn_counter;}
    //only if player shot, reload delay
    if(control.player_reload){++control.player_reload_counter;}
    //random enemy shot every second
    if((control.invader_shot_counter % config.invader_shot_period) == 0)
    {
        std ::vector<Invader>::iterator last_enemy;
        last_enemy = std ::remove_if(enemies.begin(), enemies.end(),[](Invader& enemy){return !enemy.isVisible();});
        std::uniform_int_distribution<int> dist(0, std::distance(enemies.begin(), last_enemy) - 1);
        auto index = dist(randomizer); 
        if(enemies[index].isVisible())
        {
            const auto rectangle = enemies[index].getRectangle();
            objectShot(rectangle,ShellType::Enemy);
        }
    }
    //generate invader ship spawn event
    if(((control.ship_spawn_counter % config.ship_spawn_period) == 0) && !control.invader_ship_spawned)
    {
        spawnInvaderShip();
        control.ship_spawn_counter = 0;
        control.invader_ship_spawned = true;
    }
    //player shot handle 
    if(player->getShotRequest())
    {
        player->setShotRequest(false);
        const auto rectangle = this->player->getRectangle();
        sounds.shoot_sound.play();
        objectShot(rectangle,ShellType::Player);
    }
    //player reload handle
    if(((control.player_reload_counter % config.player_reload_period) == 0) && control.player_reload)
    {
        control.player_reload = false;
    }
}

void Game::controlItemsPosition()
{
    //bullets control
    for (Shell& shell : bullets)
    {
        if(shell.isVisible())
        {
            auto position = shell.getRectangle().getPosition();
            if((position.x > default_x_size) || (position.x < default_start_x) ||
               (position.y > default_y_size) || (position.y < default_start_y)
              )
            {
                shell.setVisibility(false);
            }
        }
    }
    //enemy ship control
    if(invader_ship->isVisible())
    {
        auto position = invader_ship->getRectangle().getPosition();
        if((position.x > default_x_size) || (position.x < default_start_x) ||
           (position.y > default_y_size) || (position.y < default_start_y)
          )
        {
            sounds.ship_sound.stop();
            invader_ship->setVisibility(false);
            control.invader_ship_spawned = false;
        }
    }
    //spawn invaders again 
    if(control.invaders_left == 0)
    {
        //add logic for delay and music play
        //spawnInvaders();
    }
}

void Game::checkCollision()
{
    for (Shell& shell : bullets)
    {
        if((shell.getShellType() == ShellType::Enemy) && (shell.isVisible() == true))
        {
            //collision between enemy shells and player ship
            if(player->getRectangle().intersects(shell.getRectangle()))
            {
                handlePlayerHit();
            }
        }

        if((shell.getShellType() == ShellType::Player) && (shell.isVisible() == true))
        {
            //collision between player shells and invaders
            for (Invader& enemy : enemies)
            {

                if((enemy.isVisible() == true) && (shell.getRectangle().intersects(enemy.getRectangle()) == true))
                {
                    handleInvaderHit(shell,enemy);
                    if(control.invaders_left == 0){break;}
                }
            }
            //collision between player shells and invader ship
            if((invader_ship->isVisible() == true) && (shell.getRectangle().intersects(invader_ship->getRectangle()) == true))
            {
                handleShipHit(shell);
            }
        }
        //collision between shells and player obstacles
        for (Obstacle& obstacle : obstacles)
        {
            if((obstacle.isVisible() == true)&& (shell.getRectangle().intersects(obstacle.getRectangle()) == true))
            {
                obstacle.setVisibility(false);
                shell.setVisibility(false);
            }
        }
    }
}

void Game::executeEvent(const sf::Event &event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            status = GameStatus::Closed;
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
                    player->setMotionVector(sf::Vector2f(bottom_right_x - static_cast<float>(player->getSprite().getTextureRect().width),bottom_right_y));
                    break;

                case sf::Keyboard::Key::Space:
                    switch(status)
                    {
                        case GameStatus::NotStarted:
                            gameRestart();
                            break;
                        
                        case GameStatus::Running:
                            if(control.player_reload == false)
                            {
                                player->setShotRequest(true);
                                control.player_reload = true;
                            }
                            break;
                        
                        case GameStatus::GameOver:
                            status = GameStatus::NotStarted;
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

void Game::handlePlayerHit()
{
    //remove all shells from canvas
    for (Shell& shell : bullets){shell.setVisibility(false);}
    if(elements.player_lives > 0)
    {
        sounds.player_killed_sound.play();
        //decrease player lives counter
        --elements.player_lives;
        //move player to default position
        player->setDefaultPosition();
        player->setMotionVector(sf::Vector2f(bottom_left_x,bottom_left_y));
        control.invader_shot_counter = 0;
    }
    else{status = GameStatus::GameOver;}
}

void si::Game::handleShipHit(Shell &shell)
{
    shell.setVisibility(false);
    invader_ship->setVisibility(false);
    sounds.ship_sound.stop();
    control.invader_ship_spawned = false;
    elements.score += invader_ship_reward;
}

void si::Game::handleInvaderHit(Shell &shell, Invader &invader)
{
    if(control.invaders_left > 0)
    {
        shell.setVisibility(false);
        invader.setVisibility(false);
        sounds.invader_killed_sound.play();
        control.invaders_left--;
        elements.score += invader_reward;
    }
}

void Game::spawnInvaderShip()
{
    invader_ship->setDefaultPosition();
    invader_ship->setVisibility(true);
    sounds.ship_sound.play();
}

void Game::objectShot(const sf::FloatRect &rectangle, const ShellType shell_type)
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
        it->setVisibility(true);
    }
    else
    {
        //create new one
        Shell shell(position,config.shell_speed,shell_type);
        //we expect that one shell always exist in bullets vector
        const sf::Texture* shell_texture = bullets[0].getSprite().getTexture();
        const sf::Color shell_color      = bullets[0].getSprite().getColor();
        shell.setTexture(*shell_texture);
        shell.setSpriteColor(shell_color);
        bullets.push_back(shell);
    }
}
