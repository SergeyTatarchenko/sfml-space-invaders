/**
 * @file object.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

class Object
{
    public:
        /// @brief setup initial object position
        /// @param position vector with coordinates
        void setInitPosition(const sf::Vector2f& position){def_position = position;}
        /// @brief setup object position
        /// @param position vector with coordinates
        void setPosition(const sf::Vector2f& position){sprite.setPosition(position);}
        /// @brief set default position from def_x and def_y
        void setDefaultPosition(){sprite.setPosition(def_position);}
        /// @brief setup object speed
        /// @param speed new speed
        void setSpeed(const float speed){this->speed = speed;}
        /// @brief load and setup texture for this object
        /// @param texture reference to texture
        void setTexture(const sf::Texture& texture){sprite.setTexture(texture);}
        /// @brief setup sprite rectangle
        /// @param rectangle reference to expected rectangle
        void setSpriteRectangle(const sf::IntRect& rectangle){sprite.setTextureRect(rectangle);}
        /// @brief change sprite color
        /// @param color new color and transparency level
        void setSpriteColor(const sf::Color& color){sprite.setColor(color);}
        /// @brief set object visibility
        /// @param visibility visible or not
        void setVisibility(const bool visibility){visible = visibility;}
        /// @brief get default object position
        /// @return vector with default position
        sf::Vector2f getDefaultPosition() const {return def_position;}
        /// @brief request for object speed
        /// @return actual object speed
        float getSpeed() const {return speed;}
        /// @brief get actual object outline
        /// @return actual  object outline
        sf::FloatRect getRectangle() const {return sprite.getGlobalBounds();}
        /// @brief get reference to object sprite
        /// @return reference to sf::Sprite type class member
        sf::Sprite getSprite() const {return sprite;}
        /// @brief check if object visible or not
        /// @return true if visible false if not
        bool isVisible() const {return visible;}
        /// @brief move object sprite
        /// @param vector the vector along which we will move
        void move(const sf::Vector2f vector){sprite.move(vector);}

    private:
        /// @brief object sprite with texture
        sf::Sprite sprite;
        /// @brief default object coordinates
        sf::Vector2f def_position;
        /// @brief object state flag, whether it will be displayed in coordinates or not.
        bool visible;
        /// @brief object speed in coordinates per second
        float speed;
};

#endif //OBJECT_H
