/**
 * @file object.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

class Object
{
    public:
        void setInitPosition(const sf::Vector2f& position);
        /// @brief  setup object position
        /// @param x X coordinate of the new position
        /// @param y Y coordinate of the new position
        void setPosition(const sf::Vector2f& position);
        /// @brief set default position from def_x and def_y
        void setDefaultPosition();
        /// @brief get default object position
        /// @return vector with default position
        sf::Vector2f getDefaultPosition();
        /// @brief request for object speed
        /// @return actual object speed
        float getSpeed();
        /// @brief setup object speed
        /// @param speed 
        void setSpeed(const float speed);
        /// @brief load and setup texture for this object
        /// @param texture reference to texture
        void setTexture(const sf::Texture &texture);
        /// @brief setup sprite rectangle
        /// @param rectangle reference to expected rectangle
        void setSpriteRectangle(const sf::IntRect& rectangle);
        /// @brief change sprite color
        /// @param color new color and transparency level
        void setSpriteColor(const sf::Color &color);
        /// @brief set object as visible on the canvas 
        void setVisible();
        /// @brief set object as invisible on the canvas
        void setInvisible();
        /// @brief check if object visible or not
        /// @return true if visible false if not
        bool isVisible();
        /// @brief move object sprite
        /// @param vector the vector along which we will move
        void move(const sf::Vector2f vector);
        /// @brief get actual object outline
        /// @return const reference to actual outline
        sf::FloatRect getRectangle();
        /// @brief get reference to object sprite
        /// @return reference to sf::Sprite type class member
        sf::Sprite& getSprite();

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
