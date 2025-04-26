#include "Components.cpp"

class Entity
{
private:
    std :: string e_tag = "";
    int e_id = 0;
    bool e_status = 0;
    
public:
    Entity(std :: string t = "", int i = 0, bool s = 0)
        : e_tag(t), e_id(i), e_status(s){}
    // Bits:         5           4           3         2         1          0 
    // Components  Score    collosion(r)   input    lifespan   shape    Transform
    unsigned char ComponentsFlag = 0; 
    cShape* shape = NULL;
    cTransform* transform = NULL;
    cScore* score = NULL;
    cLifespan* lifespan = NULL;
    cInput* input = NULL; 
    cCollision* collosion = NULL;
    
    // Getters for private members

    void destroy() {e_status = 0;}
    void add() {e_status = 1;}
    bool isActive() {return e_status;}
    std::string tag() {return e_tag;}
    int id() {return e_id;}
    ~Entity();

};

Entity :: ~Entity()
{
    delete this->shape;
    delete this->transform;
    delete this->input;
    delete this->collosion;
    delete this->lifespan;
}
