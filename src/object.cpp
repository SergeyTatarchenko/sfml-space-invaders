/**
 * @file object.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "object.hpp"

void Object::setInitPosition(const sf::Vector2f &position)
{
    def_position = position;
}

void Object::setPosition(const sf::Vector2f &position)
{
    sprite.setPosition(position);
}

void Object::setDefaultPosition()
{
    sprite.setPosition(def_position);
}

sf::Vector2f Object::getDefaultPosition()
{
    return def_position;
}

float Object::getSpeed()
{
    return speed;
}

void Object::setSpeed(const float speed)
{
    this->speed = speed;
}

bool Object::setTexture(const std::string &filename)
{
    sprite.setTexture(texture);
    return true;
}

void Object::setSpriteRectangle(const sf::IntRect &rectangle)
{
    sprite.setTextureRect(rectangle);
}

void Object::setVisible()
{
    visible = true;
}

void Object::setInvisible()
{
    visible = false;
}

bool Object::isVisible()
{
    return visible;
}

void Object::move(const sf::Vector2f vector)
{
    sprite.move(vector);
}

sf::FloatRect Object::getRectangle()
{
    return sprite.getGlobalBounds();
}

sf::Sprite& Object::getSprite()
{
    return sprite;
}
