/**
 * @file items.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#ifndef ITEMS_H
#define ITEMS_H

#include "object.hpp"

enum class ShellType
{
    Enemy,
    Player
};

enum class ItemDirection
{
    Left,
    Right,
    Up,
    Down
};

class Invader : public Object
{
    public:
        /// @brief default constructor
        /// @param position initial coordinates 
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        Invader(sf::Vector2f position, float speed, bool visible);
        /// @brief change object position according to internal trajectory function
        void updatePosition();
        /// @brief setup default position and set position_counter to 0
        void revertPosition();
    
    private:
        /// @brief actual position counter, used in updatePosition
        int position_counter = 0;
};

class InvaderShip : public Object
{
    public:
        /// @brief default constructor
        /// @param position initial coordinates 
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        InvaderShip(sf::Vector2f position, float speed, bool visible);
        /// @brief update ship move direction
        /// @param direction new direction
        void setDirection(const ItemDirection direction){this->direction = direction;}
        /// @brief change object position according to internal trajectory function
        void updatePosition();
    
    private:
        /// @brief actual ship direction
        ItemDirection direction;
};

class Shell : public Object
{
    public:
        /// @brief default constructor
        /// @param position initial coordinates 
        /// @param speed object speed on canvas
        /// @param shell_type shell type (who shot)
        Shell(sf::Vector2f position, float speed, ShellType shell_type);
        /// @brief change shell type of the instance
        /// @param new_type new shell type from ShellTypes
        void setShellType(const ShellType new_type){shell_type = new_type;}
        /// @brief check instance shell type
        /// @return enum class with ShellTypes
        ShellType getShellType() const {return shell_type;};
        /// @brief change object position according to internal trajectory function
        void updatePosition();
    
    private:
        /// @brief shell type (who shot this shell)
        ShellType shell_type;
};

class PlayerShip : public Object
{
    public:
        /// @brief default constructor
        /// @param position initial coordinates 
        /// @param speed object speed on canvas
        PlayerShip(sf::Vector2f position, float speed);
        /// @brief set player shot request
        /// @param state expected player ship shot request
        void setShotRequest(bool state){shot_request = state;}
        /// @brief set player ship motion vector 
        /// @param vector new motion vector
        void setMotionVector(const sf::Vector2f& vector){motion_vector = vector;}
        /// @brief get actual player shot request
        /// @return actual player shot request
        bool getShotRequest()const {return shot_request;}
        /// @brief change object position according to internal trajectory function
        void updatePosition();
        
    private:
        /// @brief if true player ship will shot
        bool shot_request;
        /// @brief actual point to which ship will be moved
        sf::Vector2f motion_vector;
};

class Obstacle : public Object
{
    public:    
        /// @brief default constructor
        /// @param position initial coordinates 
        Obstacle(sf::Vector2f position);    
};

#endif //ITEMS_H
