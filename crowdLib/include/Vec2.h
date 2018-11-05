#ifndef VEC2_H_
#define VEC2_H_
class Vec2
{
public:
//-----------------------------------------------------------------------------------------------------
/// @brief Default constructor.
//-----------------------------------------------------------------------------------------------------
Vec2()=default;
//-----------------------------------------------------------------------------------------------------
/// @brief Single parameter constructor.
//-----------------------------------------------------------------------------------------------------
Vec2(const float _f):
    x(_f),
    y(_f)
{}
//-----------------------------------------------------------------------------------------------------
/// @brief Double parameter constructor.
//-----------------------------------------------------------------------------------------------------
Vec2(const float _x, const float _y):
    x(_x),
    y(_y)
{}
//-----------------------------------------------------------------------------------------------------
/// @brief Default destructor.
//-----------------------------------------------------------------------------------------------------
~Vec2()=default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default copy assignment operator.
//-----------------------------------------------------------------------------------------------------
Vec2(const Vec2&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default copy assignment operator.
//-----------------------------------------------------------------------------------------------------
Vec2& operator=(const Vec2&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default move assignment operator.
//-----------------------------------------------------------------------------------------------------
Vec2& operator=(Vec2&&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default move constructor.
//-----------------------------------------------------------------------------------------------------
Vec2(Vec2&&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Addition operator.
//-----------------------------------------------------------------------------------------------------
Vec2 operator+(const Vec2& rhs){return Vec2(this->x+=rhs.x, this->y+=rhs.y);}
//-----------------------------------------------------------------------------------------------------
/// @brief Addition operator.
//-----------------------------------------------------------------------------------------------------
Vec2 operator+=(const Vec2& rhs){return Vec2(this->x+=rhs.x, this->y+=rhs.y);}
//-----------------------------------------------------------------------------------------------------
/// @brief Subtraction operator.
//-----------------------------------------------------------------------------------------------------
Vec2 operator-(const Vec2& rhs){return(this->x-=rhs.x, this->y-=rhs.y);}
//-----------------------------------------------------------------------------------------------------
/// @brief Subtraction operator.
//-----------------------------------------------------------------------------------------------------
Vec2 operator-=(const Vec2& rhs){return(this->x-=rhs.x, this->y-=rhs.y);}
//-----------------------------------------------------------------------------------------------------
/// @brief Calculate distance between this and _tgt
/// @param _tgt target position
//-----------------------------------------------------------------------------------------------------
float distance (const Vec2 *_tgt);
//-----------------------------------------------------------------------------------------------------
/// @brief Calculate distance between a and b
/// @param _a start point
/// @param _b end point
//-----------------------------------------------------------------------------------------------------
float distance(Vec2 *_a, Vec2 *_b);
//-----------------------------------------------------------------------------------------------------
/// @brief Calculate the length of a vector
/// @param _v reference vector
//-----------------------------------------------------------------------------------------------------
float magnitude(Vec2 &_v);
//-----------------------------------------------------------------------------------------------------
/// @brief Calculate the length of this vector
//-----------------------------------------------------------------------------------------------------
float magnitude();
//-----------------------------------------------------------------------------------------------------
/// @brief Normalize a vector
/// @param _v reference vector
//-----------------------------------------------------------------------------------------------------
void normalize(Vec2 *_v);
//-----------------------------------------------------------------------------------------------------
/// @brief Normalize this vector
//-----------------------------------------------------------------------------------------------------
void normalize();
//-----------------------------------------------------------------------------------------------------
/// @brief X component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float x;
//-----------------------------------------------------------------------------------------------------
/// @brief Y component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float y;
};
#endif //VEC2_H_
