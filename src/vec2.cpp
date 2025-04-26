#include <SFML/Graphics.hpp>
#include <cmath>
class vec2
{
public:
    float x;
    float y;
    vec2(){}
    vec2(const float x_c, const float y_c): x(x_c), y(y_c){}
    vec2(const vec2& obj);
    vec2(const sf::Vector2f&);
    vec2(const sf::Vector2u&);
    vec2 operator + (const vec2& obj);
    void operator += (const vec2& obj);
    vec2 operator - (const vec2& obj);
    vec2 operator / (const vec2& obj);
    vec2 operator * (const vec2& obj);

    vec2& scale (float s);
    vec2& add (const vec2& obj);
    vec2& rotate (float degree);
    float distance (const vec2& obj);
    float norm (const vec2& obj);
    float norm (const sf::Vector2f&);
    float norm (const sf::Vector2u&);

    bool operator == (const vec2& obj);
    vec2& operator = (const sf::Vector2f);
    vec2& operator = (const sf::Vector2u);
};

vec2 :: vec2(const vec2& obj): x(obj.x), y(obj.y){}
vec2 :: vec2(const sf::Vector2f& obj): x(obj.x), y(obj.y){};
vec2 :: vec2(const sf::Vector2u& obj): x(obj.x), y(obj.y){};
vec2 vec2 ::  operator + (const vec2& obj)
{
    return vec2(x+obj.x, y+obj.y);
}
void vec2 :: operator += (const vec2& obj)
{
    x += obj.x;
    y += obj.y;
}
vec2& vec2 :: add (const vec2& obj)
{
    x += obj.x;
    y += obj.y;
    return *this;
}
vec2 vec2 :: operator - (const vec2& obj)
{
    return vec2(x-obj.x, y-obj.y);
}
vec2 vec2 :: operator * (const vec2& obj)
{
    return vec2(x*obj.x, y*obj.y);
}
vec2& vec2 :: scale (float s)
{
    x *= s;
    y *= s;
    return *this;
}
vec2& vec2 :: rotate (float degree)
{
    return *this;
}
float vec2 :: distance (const vec2& obj)
{
    return sqrt((obj.x-x)*(obj.x-x)+(obj.y-y)*(obj.y-y));
}
bool vec2 ::  operator == (const vec2& obj)
{
    return x == obj.x && y == obj.y;
}
float vec2 :: norm (const vec2& obj)
{
    return (obj.x-x)*(obj.x-x)+(obj.y-y)*(obj.y-y);
}
float vec2 :: norm (const sf::Vector2f& obj)
{
    return (obj.x-x)*(obj.x-x)+(obj.y-y)*(obj.y-y);
}
float vec2 :: norm (const sf::Vector2u& obj)
{
    return (obj.x-x)*(obj.x-x)+(obj.y-y)*(obj.y-y);
}
vec2& vec2 :: operator = (const sf::Vector2f vector)
{
    x = vector.x;
    y = vector.y;
    return *this;
}
vec2& vec2 :: operator = (const sf::Vector2u vector)
{
    x = vector.x;
    y = vector.y;
    return *this;
}
