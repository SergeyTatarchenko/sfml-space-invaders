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
        /// @brief set object as visible on the canvas 
        void setVisible();
        /// @brief set object as invisible on the canvas
        void setInvisible();
        /// @brief check if object visible or not
        /// @return true if visible false if not
        bool isVisible();
        /// @brief get reference to object outline
        /// @return const reference to actual outline
        sf::FloatRect getRectangle();
        /// @brief get reference to object sprite
        /// @return const reference to sf::Sprite type class member
        const sf::Sprite& getSprite();

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
