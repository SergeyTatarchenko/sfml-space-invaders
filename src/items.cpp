/**
 * @file items.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */
#include "items.hpp"
#include <cmath>

///////////////////SETTINGS FOR ITEMS WHITHOUT TEXTURES/////////////////////////
constexpr int shell_width     = 2;
constexpr int shell_height    = 10;
constexpr int obstacle_width  = 10;
constexpr int obstacle_height = 10;
////////////////////////////////////////////////////////////////////////////////

Invader::Invader(sf::Vector2f position, float speed, bool visible)
{
    setPosition(position);
    setSpeed(speed);
    setVisibility(visible);    
}

void Invader::updatePosition()
{
    if(isVisible())
    {
        // Invader trajectory:
        //  450 steps
        // ---------->
        // |         |   
        // |         | 20 steps
        // <---------|
        const int step_x = 450;
        const int step_y = 20;

        auto speed = getSpeed();
        sf::Vector2 vector(0.f,0.f);
        if(position_counter < step_x)                     {vector.x = speed;}
        else if (position_counter < (step_x + step_y))    {vector.y = speed;}
        else if (position_counter < (2*step_x + step_y))  {vector.x = (-1.f)*speed;}
        else if (position_counter < (2*step_x + 2*step_y)){vector.y = (-1.f)*speed;}
        ++position_counter;
        move(vector);
        if(position_counter == (2*step_x + 2*step_y)){revertPosition();}
    }
}

void Invader::revertPosition()
{
    position_counter = 0;
    setDefaultPosition();
}

InvaderShip::InvaderShip(sf::Vector2f position, float speed, bool visible) : direction(ItemDirection::Right)
{
    setInitPosition(position);
    setPosition(position);
    setSpeed(speed);
    setVisibility(visible);
}

void InvaderShip::updatePosition()
{
    // Invader ship trajectory:
    // --------------->
    //        or
    // <--------------|
    if(isVisible())
    {
        sf::Vector2 vector(getSpeed(),0.f);
        if(direction == ItemDirection::Left){vector.x *= (-1.f);}
        move(vector); 
    }       
}

Shell::Shell(sf::Vector2f position, float speed, ShellType shell_type) : shell_type(shell_type)
{
    setPosition(position);
    setSpeed(speed);
    setVisibility(true);
    setSpriteRectangle(sf::IntRect(0, 0, shell_width, shell_height));
}

void Shell::updatePosition()
{
    // shell trajectory:
    // straight up or down from the creation point
    if(isVisible())
    {
        sf::Vector2 vector(0.f,getSpeed());
        if(shell_type == ShellType::Player){vector.y *= (-1.f);}    
        move(vector); 
    }     
}

PlayerShip::PlayerShip(sf::Vector2f position, float speed): shot_request(false)
{
    setPosition(position);
    setVisibility(true);
    setSpeed(speed);
}

void PlayerShip::updatePosition()
{
    auto calc_distance = []( sf::Vector2f& vector_a,  sf::Vector2f& vector_b)
    {
        // R = sqrt((B.x - A.x)^2 + (B.y - A.y)^2)
        return sqrtf(powf((vector_b.x - vector_a.x),2.f) + powf((vector_b.y - vector_a.y),2.f));
    };

    auto get_cathetuses = [](sf::Vector2f& vector_a,  sf::Vector2f& vector_b)
    {
        // return cathetuses of right-angled triangle created from vector_a and vector_b
        return sf::Vector2f((vector_b.x - vector_a.x),(vector_b.y - vector_a.y));
    };

    if(isVisible())
    {
        sf::Vector2 vector(0.f,0.f);
        auto position = getRectangle().getPosition();   
        auto speed    = getSpeed();
        if(position != motion_vector)
        {
            auto distance_left = calc_distance(position,motion_vector);
            if(distance_left <= speed)
            {
                // set the vector as the end point of the path
                setPosition(motion_vector);
            }
            else
            {
                //calc new point
                auto cathetuses = get_cathetuses(position,motion_vector);
                vector.x = (cathetuses.x/distance_left) * speed;
                vector.y = (cathetuses.y/distance_left) * speed;
                move(vector);
            }
        } 
    }     
}

Obstacle::Obstacle(sf::Vector2f position)
{
    setPosition(position);
    setVisibility(true);
    setSpriteRectangle(sf::IntRect(0, 0, obstacle_width, obstacle_height));
}
