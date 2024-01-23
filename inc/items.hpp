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
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Invader : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        Invader(float x, float y, float speed, bool visible);
        /// @brief change object position according to internal trajectory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        /// @brief object position counter, used in moveAlongTrajectory function 
        int position_counter;

};

class Shell : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param speed object speed on canvas
        /// @param shell_type shell type (who shot)
        Shell(float x, float y, float speed, ShellType shell_type);
        /// @brief check instance shell type
        /// @return enum class with ShellTypes
        ShellType getShellType();
        /// @brief change shell type of the instance
        /// @param new_type new shell type from ShellTypes
        void setShellType(const ShellType new_type);
        /// @brief change object position according to internal trajectory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        /// @brief shell type (who shot this shell)
        ShellType shell_type;
};

class InvaderShip : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        InvaderShip(float x, float y, float speed, bool visible, float range);
        /// @brief change object position according to internal trajectory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
        /// @brief set new ship direction to move on its range
        /// @param direction new direction 
        void setDirection(const ItemDirection direction);
    
    private:
        /// @brief used in object trajectory
        float range;
        /// @brief previous item direction on canvas
        ItemDirection direction;
};

class PlayerShip : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param limit_x limit for move for x asis
        /// @param limit_y limit for move for y asis
        PlayerShip(float x, float y, float speed, float limit_x, float limit_y);
        /// @brief move ship to selected direction
        /// @param direction new direction 
        void setDirection(ItemDirection direction);
        void moveAlongTrajectory(unsigned int framerate);
        void setShotRequest(bool state);
        bool getShotRequest();
    private:
        bool invincible;
        bool shot_request;
        ItemDirection direction;
        float limit_x;
        float limit_y;
};

#endif //ITEMS_H
