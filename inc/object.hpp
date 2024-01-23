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
        /// @brief  setup object position
        /// @param x X coordinate of the new position
        /// @param y Y coordinate of the new position
        void setPosition(const float x, const float y);
        /// @brief set default position from def_x and def_y
        void setDefaultPosition();
        /// @brief setup common object parameters
        /// @param speed object speed
        /// @param visible object visibility
        void setParameters(const float speed, const bool visible);
        /// @brief load and setup texture for this object
        /// @param filename path and texture filename
        /// @return true in case of success false if not
        bool setTexture(const std::string &filename);
        /// @brief setup sprite rectangle
        /// @param rectangle reference to expected rectangle
        void setSpriteRectangle(const sf::IntRect& rectangle);
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

    protected:
        /// @brief object sprite with texture
        sf::Sprite sprite;
        /// @brief object with loaded texture
        sf::Texture texture;
        /// @brief object state flag, whether it will be displayed in coordinates or not.
        bool visible;
        /// @brief object speed in coordinates per second
        float speed;
        /// @brief default object x coordinate    
        float def_x;
        /// @brief default object y coordinate
        float def_y;
};

#endif //OBJECT_H
