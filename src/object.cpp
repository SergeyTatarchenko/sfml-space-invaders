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

void Object::setPosition(const float x, const float y)
{
    def_x = x;
    def_y = y;
    sprite.setPosition(x,y);
}

void Object::setParameters(const float speed, const bool visible)
{
    this->speed   = speed;
    this->visible = visible;
}

void Object::setDefaultPosition()
{
    sprite.setPosition(def_x,def_y);
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
