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
#include "canvas.hpp"

//window title
static const sf::String title = "Space Invaders";
//game version
static const sf::String version = "0.01";
//object rectangles for the menu frames
std::array<sf::IntRect,num_of_frames> frame_rectangles = 
{
    sf::IntRect(0,0,si::default_x_size,si::frame_width),
    sf::IntRect(0,0,si::frame_width,si::default_y_size),
    sf::IntRect(0,0,si::default_x_size,si::frame_width),
    sf::IntRect(0,0,si::frame_width,si::default_y_size),
    sf::IntRect(0,0,si::default_x_size,si::frame_width)
};
//object positions for the menu frames
std::array<sf::Vector2f,num_of_frames> frame_positions = 
{
    sf::Vector2f(si::default_start_x,si::default_start_y),
    sf::Vector2f(si::default_x_size - static_cast<float>(si::frame_width),si::default_start_y),
    sf::Vector2f(si::default_start_x,static_cast<float>(si::frame_length)),
    sf::Vector2f(si::default_start_x,si::default_start_y),
    sf::Vector2f(si::default_start_x,si::default_y_size - static_cast<float>(si::frame_width))
};

Canvas::Canvas(const unsigned int framerate):
                window(sf::VideoMode(canvas_width, canvas_height), title),
                game(si::Game(framerate))
{
    sf::View view(sf::FloatRect(si::default_start_x, si::default_start_y, si::default_x_size, si::default_y_size));
    window.setView(view);
    window.setActive(true);
    window.setFramerateLimit(framerate);
    loadResources();
    setupTextures();
    setupSounds();
    setupMenu();
}

void Canvas::runEventLoop()
{
    sf::Event event;
    while (window.isOpen())
    {
        while(window.pollEvent(event)){game.executeEvent(event);}
        window.clear(sf::Color::Black);
        switch(game.status)
        {
            case si::GameStatus::NotStarted:
                drawWelcomeWindow();
                break;
            
            case si::GameStatus::Running:
                game.gameLoop();
                menu_sprites.score.setString("SCORE: " + std::to_string(game.elements.score));
                updateCanvas();
                break;
            
            case si::GameStatus::GameOver:
                drawGameOverScreen();
                break;
            case si::GameStatus::Closed:    
            default:
                window.close();
                break;
        }
        window.display();
    }
}

void Canvas::updateCanvas()
{
    //update score indicator
    window.draw(menu_sprites.score);
    //update lives indicator
    drawPlayerLives();
    //update menu frames
    for(Object & frame : menu_sprites.frames){window.draw(frame.getSprite());}
    //update enemies 
    for (Invader& enemy : game.enemies)
    {
        if(enemy.isVisible()){window.draw(enemy.getSprite());}
    }    
    //update bullets
    for (Shell& shell : game.bullets)
    {
        if(shell.isVisible()){window.draw(shell.getSprite());}
    }
    //update obstacles
    for (Obstacle& obstacle : game.obstacles)
    {
        if(obstacle.isVisible()){window.draw(obstacle.getSprite());}
    }
    //update enemy ship
    if(game.invader_ship->isVisible()){window.draw(game.invader_ship->getSprite());}
    //update player ship
    window.draw(game.player->getSprite());
}

void Canvas::setupMenu()
{
    //setup game score item
    menu_sprites.score.setFont(resources.game_font);
    menu_sprites.score.setCharacterSize(font_size);
    menu_sprites.score.setPosition(sf::Vector2f(static_cast<float>(si::frame_length),static_cast<float>(si::frame_width)));
    //player lives indicator
    menu_sprites.live.setTexture(resources.player);
    //setup canvas frames
    for (Object& frame: menu_sprites.frames)
    {
        auto i = &frame - &menu_sprites.frames[0];
        frame.setSpriteRectangle(frame_rectangles[i]);
        frame.setTexture(resources.frame);
        frame.setSpriteColor(sf::Color::White);
        frame.setPosition(frame_positions[i]);
    }
}

void Canvas::drawPlayerLives()
{
    //small border to prevent sprites from sticking together
    constexpr float border = 10.f;
    sf::IntRect texture_size =  menu_sprites.live.getTextureRect();
    //initial offset, lives will be drawn from right to left
    float offset = si::default_x_size - si::frame_width - static_cast<float>(texture_size.width) - border;
    for(auto i = 0; i < game.elements.player_lives; i++)
    {
        menu_sprites.live.setPosition(sf::Vector2f(offset,static_cast<float>(si::frame_width)));
        window.draw(menu_sprites.live);
        offset -= static_cast<float>(texture_size.width) + border;
    }
}

void Canvas::drawWelcomeWindow()
{
    sf::Text welcome_text;
    sf::Vector2f position(si::default_border_size,si::default_border_size);
    welcome_text.setFont(resources.game_font);
    welcome_text.setCharacterSize(font_size);

    welcome_text.setString(title + " version : " + version);
    welcome_text.setPosition(position);
    window.draw(welcome_text);
    position.y += si::default_border_size;

    welcome_text.setString("Controls:");
    welcome_text.setPosition(position);
    window.draw(welcome_text);
    position.y += si::default_border_size;

    welcome_text.setString("Shot - Space Bar");
    welcome_text.setPosition(position);
    window.draw(welcome_text);
    position.y += si::default_border_size;

    welcome_text.setString("Movement - arrows");
    welcome_text.setPosition(position);
    window.draw(welcome_text);
    position.y += si::default_border_size;
    
    welcome_text.setString("Press Space key to start...");
    welcome_text.setPosition(position);
    window.draw(welcome_text);
}

void Canvas::drawGameOverScreen()
{
    sf::Text text;
    sf::Vector2f position(si::default_border_size,si::default_border_size);
    text.setFont(resources.game_font);
    text.setCharacterSize(font_size);

    text.setString("GAME OVER");
    text.setPosition(position);
    window.draw(text);
    position.y += si::default_border_size;

    text.setString("Your score : " + std::to_string(game.elements.score));
    text.setPosition(position);
    window.draw(text);
    position.y += si::default_border_size;

    text.setString("Press Space key to restart the game");
    text.setPosition(position);
    window.draw(text);
}

void Canvas::loadResources()
{
    //textures
    if(!resources.enemy_type_1.loadFromFile("rc/textures/green.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resources.enemy_type_2.loadFromFile("rc/textures/red.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resources.enemy_type_3.loadFromFile("rc/textures/yellow.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resources.player.loadFromFile("rc/textures/player.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    if(!resources.enemy_ship.loadFromFile("rc/textures/extra.png"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //font
    if(!resources.game_font.loadFromFile("rc/fonts/SpaceMission.ttf"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //shoot sound
    if(!resources.shoot_sound_buffer.loadFromFile("rc/sounds/shoot.wav"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //invader killed sound
    if(!resources.invader_killed_sound_buffer.loadFromFile("rc/sounds/invaderkilled.wav"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //player killed sound
    if(!resources.player_killed_sound_buffer.loadFromFile("rc/sounds/explosion.wav"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    //invader ship sound
    if(!resources.ship_sound_buffer.loadFromFile("rc/sounds/ufo_highpitch.wav"))
    {
        throw std::runtime_error(std::string("Could not load resource files!"));
    }
    resources.player.setSmooth(true);
    resources.enemy_ship.setSmooth(true);
    resources.enemy_type_1.setSmooth(true);
    resources.enemy_type_2.setSmooth(true);
    resources.enemy_type_3.setSmooth(true);
}

void Canvas::setupSounds()
{
    game.sounds.shoot_sound.setBuffer(resources.shoot_sound_buffer);
    game.sounds.invader_killed_sound.setBuffer(resources.invader_killed_sound_buffer);
    game.sounds.player_killed_sound.setBuffer(resources.player_killed_sound_buffer);
    game.sounds.ship_sound.setBuffer(resources.ship_sound_buffer);
    //shall be played during the time when ship is present on the canvas
    game.sounds.ship_sound.setLoop(true);
}

void Canvas::setupTextures()
{
    auto set_texture = [this](Invader &invader, int index)
    {
        switch (index)
        {
            case 1:
            case 4:
                invader.setTexture(resources.enemy_type_2);
                break;
            case 2:
            case 5:
                invader.setTexture(resources.enemy_type_3);
                break;
            case 0:
            case 3:
            default:
                invader.setTexture(resources.enemy_type_1);
                break;
        }
    };

    for(Obstacle& obstacle : game.obstacles){obstacle.setTexture(resources.obstacle);}
    game.player->setTexture(resources.player);
    game.invader_ship->setTexture(resources.enemy_ship);
    //set texture and color for first shell in array
    game.bullets[0].setTexture(resources.shell);
    game.bullets[0].setSpriteColor(sf::Color(40, 236, 250));
    //special texture setup for invaders (use different textures for different rows)
    int row_counter = 0;
    int num_of_invaders = game.enemies.size();
    for(auto i = 0; i <num_of_invaders; ++i)
    {
        set_texture(game.enemies[i],row_counter);
        if(((i+1) % si::invaders_in_row) == 0){row_counter++;}
    }
}
