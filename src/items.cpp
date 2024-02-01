/**
 * @file items.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "items.hpp"
 #include <cmath>

//default shell dimensions
constexpr int shell_width         = 2;
constexpr int shell_height        = 10;

Invader::Invader(sf::Vector2f position, float speed, bool visible)
{
    position_counter = 0;
    setPosition(position);
    setSpeed(speed);
    if(visible == true){setVisible();}
    else{setInvisible();}    
}

void Invader::updatePosition()
{
    if(isVisible() == true)
    {
        // Invader trajectory:
        //  60 steps
        // ---------->
        // |         |   
        // |         | 20 steps
        // <---------|
        const int step_x = 60;
        const int step_y = 20;
        auto speed = getSpeed();
        sf::Vector2 vector(0.f,0.f);

        if(position_counter < step_x)                     {vector.x = speed;}
        else if (position_counter < (step_x + step_y))    {vector.y = speed;}
        else if (position_counter < (2*step_x + step_y))  {vector.x = (-1.f)*speed;}
        else if (position_counter < (2*step_x + 2*step_y)){vector.y = (-1.f)*speed;}
        
        position_counter++;
        move(vector);
        if(position_counter == (2*step_x + 2*step_y))
        {
            position_counter = 0;
            setDefaultPosition();    
        }
    }
}

InvaderShip::InvaderShip(sf::Vector2f position, float speed, bool visible, float range)
{
    //start with move right
    direction   = ItemDirection::RIGHT;
    this->range = range;
    setPosition(position);
    setSpeed(speed);
    if(visible == true){setVisible();}
    else{setInvisible();}
}

void InvaderShip::updatePosition()
{
    // Invader ship trajectory:
    //  n steps, depends on this->range
    // --------------->
    // <--------------|
    if(isVisible() == true)
    {
        auto def_position = getDefaultPosition();
        auto rectangle    = getRectangle(); 
        auto position     = rectangle.getPosition();
        auto speed        = getSpeed();
        sf::Vector2 vector(speed,0.f);

        //turn back in case of reaching end of range
        if((position.x + speed + rectangle.width) > (def_position.x + range)){direction = ItemDirection::LEFT;}
        else if((position.x - speed) < def_position.x){this->direction = ItemDirection::RIGHT;}
        switch(this->direction)
        {
            case ItemDirection::LEFT:
                vector.x *= (-1.f);
                break;
            default:
                break;
        }    
        move(vector); 
    }       
}

Shell::Shell(sf::Vector2f position, float speed, ShellType shell_type)
{
    this->shell_type = shell_type;
    setPosition(position);
    setSpeed(speed);
    setVisible();
    setSpriteRectangle(sf::IntRect(0, 0, shell_width, shell_height));
}

ShellType Shell::getShellType()
{
    return shell_type;
}

void Shell::setShellType(const ShellType new_type)
{
    shell_type = new_type;
}

void Shell::updatePosition()
{
    // shell trajectory:
    // straight up or down from the creation point
    if(isVisible() == true)
    {
        auto speed        = getSpeed();
        sf::Vector2 vector(0.f,speed);

        switch(shell_type)
        {
            case ShellType::PLAYER:
                vector.y *= (-1.f);
                break;
            default:
                break;
        }    
        move(vector); 
    }     
}

PlayerShip::PlayerShip(sf::Vector2f position,sf::Vector2f limits, float speed)
{

    this->limits       = limits;
    invincible   = true;
    shot_request = false;
    setPosition(position);
    setVisible();
    setSpeed(speed);
}

void PlayerShip::setShotRequest(bool state)
{
    shot_request = state;
}

bool PlayerShip::getShotRequest()
{
    return shot_request;
}

void PlayerShip::setMotionVector(const sf::Vector2f &vector)
{
    motion_vector = vector;
}

void PlayerShip::updatePosition()
{
    auto calc_distance = []( sf::Vector2f &vector_a,  sf::Vector2f &vector_b)
    {
        // R = sqrt((B.x - A.x)^2 + (B.y - A.y)^2)
        return sqrtf(powf((vector_b.x - vector_a.x),2.f) + powf((vector_b.y - vector_a.y),2.f));
    };

    auto get_cathetuses = [](sf::Vector2f &vector_a,  sf::Vector2f &vector_b)
    {
        // return cathetuses of right-angled triangle created from vector_a and vector_b
        return sf::Vector2f((vector_b.x - vector_a.x),(vector_b.y - vector_a.y));
    };

    if(isVisible() == true)
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
    
}
