#ifndef CROWDLIB_COLOUR_H_
#define CROWDLIB_COLOUR_H_
class Colour
{
public:
//-----------------------------------------------------------------------------------------------------
/// @brief Default constructor.
//-----------------------------------------------------------------------------------------------------
Colour():
    r(1.f),
    g(1.f),
    b(1.f),
    a(1.f)
{}
//-----------------------------------------------------------------------------------------------------
/// @brief Single parameter constructor.
//-----------------------------------------------------------------------------------------------------
Colour(const float _f):
    r(_f),
    g(_f),
    b(_f),
    a(1.f)
{}
//-----------------------------------------------------------------------------------------------------
/// @brief Quadruple parameter constructor.
//-----------------------------------------------------------------------------------------------------
Colour(const float _r, const float _g, const float _b, const float _a):
    r(_r),
    g(_g),
    b(_b),
    a(_a)
{}
//-----------------------------------------------------------------------------------------------------
/// @brief Default destructor.
//-----------------------------------------------------------------------------------------------------
~Colour()=default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default copy assignment operator.
//-----------------------------------------------------------------------------------------------------
Colour(const Colour&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default copy assignment operator.
//-----------------------------------------------------------------------------------------------------
Colour& operator=(const Colour&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default move assignment operator.
//-----------------------------------------------------------------------------------------------------
Colour& operator=(Colour&&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Default move constructor.
//-----------------------------------------------------------------------------------------------------
Colour(Colour&&) = default;
//-----------------------------------------------------------------------------------------------------
/// @brief Addition operator.
//-----------------------------------------------------------------------------------------------------
Colour operator+(const Colour& rhs){return Colour(this->r+=rhs.r, this->g+=rhs.g, this->b+=rhs.b, this->a+=rhs.a);}
//-----------------------------------------------------------------------------------------------------
/// @brief Addition operator.
//-----------------------------------------------------------------------------------------------------
Colour operator+=(const Colour& rhs){return Colour(this->r+=rhs.r, this->g+=rhs.g, this->b+=rhs.b, this->a+=rhs.a);}
//-----------------------------------------------------------------------------------------------------
/// @brief Subtraction operator.
//-----------------------------------------------------------------------------------------------------
Colour operator-(const Colour& rhs){return Colour(this->r-=rhs.r, this->g-=rhs.g, this->b-=rhs.b, this->a-=rhs.a);}
//-----------------------------------------------------------------------------------------------------
/// @brief Subtraction operator.
//-----------------------------------------------------------------------------------------------------
Colour operator-=(const Colour& rhs){return Colour(this->r-=rhs.r, this->g-=rhs.g, this->b-=rhs.b, this->a-=rhs.a);}
//-----------------------------------------------------------------------------------------------------
/// @brief Red component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float r;
//-----------------------------------------------------------------------------------------------------
/// @brief Green component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float g;
//-----------------------------------------------------------------------------------------------------
/// @brief Blue component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float b;
//-----------------------------------------------------------------------------------------------------
/// @brief Alpha component of this vector. Single letter name for visually easy access in code
//-----------------------------------------------------------------------------------------------------
float a;
};
#endif //CROWDLIB_COLOUR_H_
