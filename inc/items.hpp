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

#define INVADER_WIDTH        (int)(20)
#define INVADER_HEIGHT       (int)(20)

#define INVADER_SHIP_WIDTH   (int)(40)
#define INVADER__SHIP_HEIGHT (int)(20)

#define SHELL_WIDTH          (int)(5)
#define SHELL_HEIGHT         (int)(5)

enum class ShellTypes
{
    ENEMY_SHELL,
    PLAYER_SHELL
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
        Shell(float x, float y, float speed, ShellTypes shell_type);
        /// @brief check instance shell type
        /// @return enum class with ShellTypes
        const ShellTypes getShellType();
        /// @brief update shell speed
        /// @param speed new speed
        void setSpeed(const float speed);
        /// @brief change shell type of the instance
        /// @param new_type new shell type from ShellTypes
        void setShellType(const ShellTypes new_type);
        /// @brief change object position according to internal trajectory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        /// @brief shell type (who shot this shell)
        ShellTypes shell_type;
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
    
    private:
        /// @brief used in object trajectory
        float range;
        /// @brief previous item direction on canvas
        ItemDirection direction;
};

#endif //ITEMS_H
