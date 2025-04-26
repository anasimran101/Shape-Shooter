#include "vec2.cpp"

//Transformation class for speed and position

class cTransform
{
public:
    vec2 speed {0,0};
    vec2 position {0,0};
    float angle = 0;
    cTransform(const vec2 s,const vec2 p):speed(s), position(p){}
};

class cShape
{
public:
    sf::CircleShape shape;
    float Transparecny;
    cShape(const sf::Color& Inner,const sf::Color& OutlineColor,const vec2& p, int OutlineTickness, int Points, int radius)
        :shape(radius)
    {
        shape.setFillColor(Inner);
        shape.setOutlineColor(OutlineColor);
        shape.setOutlineThickness(OutlineTickness);
        shape.setPointCount(Points);
        shape.setOrigin(radius,radius);
        shape.setPosition(p.x, p.y);
    }
};

// class cTexture
// {
// public:
    
// };

class cCollision
{
public:
    float radius = 0;
    cCollision(float r = 0):radius(r){};
};

class cScore
{
public:
    int score = 0;
    cScore(int c = 0):score(c){};
};

class cLifespan
{
public:
    int remaining;
    int cooldown;
    cLifespan(int c = 0):remaining(c),cooldown(c){}
};

class cInput
{
public:
    bool Up = 0;
    bool Down = 0;
    bool Left = 0;
    bool Right = 0;
    bool MouseClick = 0;
    bool M = 0;
    vec2 MousePosition{-1,-1};
    void flush(){Up = Down = Left = Right = MouseClick = M = 0;}
};

