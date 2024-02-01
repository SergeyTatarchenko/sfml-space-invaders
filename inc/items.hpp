/**
 * @file items.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#ifndef ITEMS_H
#define ITEMS_H

#include "object.hpp"

enum class ShellType
{
    ENEMY,
    PLAYER
};

enum class ItemDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN
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
    
    private:
        int position_counter;
};

class InvaderShip : public Object
{
    public:
        /// @brief default constructor
        /// @param position initial coordinates 
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        InvaderShip(sf::Vector2f position, float speed, bool visible, float range);
        /// @brief change object position according to internal trajectory function
        void updatePosition();
    
    private:
        /// @brief used in object trajectory
        float range;
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
        /// @brief check instance shell type
        /// @return enum class with ShellTypes
        ShellType getShellType();
        /// @brief change shell type of the instance
        /// @param new_type new shell type from ShellTypes
        void setShellType(const ShellType new_type);
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
        /// @param limits move limits on canvas 
        /// @param speed object speed on canvas
        PlayerShip(sf::Vector2f position,sf::Vector2f limits, float speed);
        /// @brief 
        /// @param state 
        void setShotRequest(bool state);
        /// @brief 
        /// @return 
        bool getShotRequest();
        /// @brief 
        /// @param vector 
        void setMotionVector(const sf::Vector2f& vector);
        /// @brief change object position according to internal trajectory function
        void updatePosition();
        
    private:
        /// @brief 
        bool invincible;
        /// @brief 
        bool shot_request;
        /// @brief 
        sf::Vector2f limits;
        /// @brief 
        sf::Vector2f motion_vector;
};

struct Obstacle : public Object
{
        /// @brief default constructor
        /// @param position initial coordinates 
        Obstacle(sf::Vector2f position);    
};

#endif //ITEMS_H
