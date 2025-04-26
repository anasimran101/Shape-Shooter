#include <map>
#include "Entity.cpp"

class EntityManager 
{
private:
    std::vector <Entity*> entities;
    std::vector <Entity*> toAdd;
    std::map<std::string, std::vector <Entity*>> m_map;
    static const std::vector <std::string> tags;  
public:
    EntityManager(){}
    void addEntity(Entity*&);
    void update();
    std::vector <Entity*>& getEntities();
    std::vector <Entity*>& getEntities(const std::string); 
    ~EntityManager();
};

const std::vector<std::string> EntityManager::tags = {"player", "enemy", "bullet"};

void EntityManager::addEntity(Entity*& obj)
{
    toAdd.push_back(obj);
}
void EntityManager::update()
{
    for(auto& e : toAdd)
    {
        entities.push_back(e);
        m_map[e->tag()].push_back(e);
    }
    toAdd.clear();

    //Destroy Inactive Enteties

    size_t copyindex = 0, i = 0;
    while(copyindex < entities.size())
    {
        if(entities[copyindex]->isActive()) 
            entities[i++] = entities[copyindex];
        
        copyindex++;
        
    }
    entities.resize(i); 
    for(auto& t:tags)
    {
        copyindex = 0, i = 0;
        while(copyindex < m_map[t].size())
        {
            if( m_map[t][copyindex]->isActive()) 
                m_map[t][i++] = m_map[t][copyindex++];
            else
                delete m_map[t][copyindex++];
            
        }
        m_map[t].resize(i); 
    }
    
}

std::vector <Entity*>& EntityManager::getEntities()
{return entities;}

std::vector <Entity*>& EntityManager:: getEntities(const std::string tag) 
{return m_map[tag];}


EntityManager :: ~EntityManager()
{
    for(auto e: entities) 
    {
        delete e;
    }

}
