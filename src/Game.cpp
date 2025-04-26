#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "EntityManager.cpp"


/*Change cpp file paths before compilation*/

/* Install Sfml before compilation
 
Command: g++ -c Game.cpp && g++ Game.o -o Shape-Shooter -lsfml-graphics -lsfml-window -lsfml-system && ./Shape-Shooter

*/

/***** Playing Instructions *****

    Press Escape to Pause.
    Use the Arrow Keys to move the Player.
    Press Space to use the Special Move.
    An asterisk (*) behind "Lives" indicates the availability of the Special Move.
    Edit config.txt to control graphics settings.
    After a life is lost, the Player is invincible until its opacity fully increases.


<<<<<<<< Config file Layout >>>>>>

Window 1280 720 60 0
Player 32 32 15 50 50 50 255 0 0 1 9
Enemy 32 32 3 5 255 255 255 255 255 255 2 3 8 1440 180
Bullet 5 5 20 255 0 0 255 255 255 2 20 60


Window Specifications:
Window W H FR FS
Width           W               int
Height          H               int
Frame Rate      FR              int
Full Screen     FS              bool

Player Specifications:
Player SR CR S FR FG FB OR OG OB OT V
Shape Radius        SR              int
Collision Radius    CR              int
Speed               S               float
Fill Color          FR, FG, FB      int, int, int
Outline Color       OR, OG, OB      int, int, int
Outline Thickness   OT              int
Shape Vertices      V               int

Enemy Specification:
Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
Shape Radius        SR              int
Collision Radius    CR              int
Speed Min Max       SMIN SMAX       float, float
Fill Color          FR, FG, FB      int, int, int
Outline Color       OR, OG, OB      int, int, int
Outline Thickness   OT              int
Shape Vertices      VMIN VMAX       int, int
Lifespan            L               int
Respawn             SP              int

Bullet Specification:
Bullet SR CR S FR FG FB OR OG OB OT V L
Shape Radius        SR              int
Collision Radius    CR              int
Speed               S               float
Fill Color          FR, FG, FB      int, int, int
Outline Color       OR, OG, OB      int, int, int
Outline Thickness   OT              int
Shape Vertices      V               int
Lifespan            L               int

*/

std::string configerations = 
"Window 1280 720 60 0\n"
"Player 32 32 15 50 50 50 255 0 0 1 9\n"
"Enemy 32 32 3 5 0 255 0 0 0 0 2 3 8 1440 180\n"
"Bullet 5 5 20 255 0 0 255 255 255 2 20 60";



/*[CHECK] lives not decreasing in display */







#define TotalLives 3
#define RespawnCooldownSeconds 3
#define TextSize 20
#define footerTopHeight 1
#define footerBottomHeight 30 
#define maxBullets 3      
#define ScorePerVertex 10
#define SpecialMoveCooldownSec 15
#define maxSpecialMoveBullets 15
#define MinSpecialMoveBullets 8
#define GameOverTextYPosition 720/3
#define playerRotation 2

struct Window
{
    int W, H, FR;
    bool FS;
};

struct Font
{
    int F, S, R, G, B;
};

struct Vars
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, VMIN, VMAX, L, SP;
    float S, SMIN, SMAX; 
};


/***************For Installation**************/
std::string filesDirectory = "/home/"+std::string(getenv("USER"))+"/Documents/Shape-Shooter/";

/***********For-Testing*********/

//std::string filesDirectory = "dat/";

std::string playerTexturePath =  filesDirectory + "ufo.png";
std::string fontPath =           filesDirectory + "Arial.ttf";
std::string backgroundPath =     filesDirectory + "background.jpg";
std::string configPath =         filesDirectory + "config.txt";
std::string scoreFilePath =      filesDirectory + "scores.txt";




class Game
{
private:

    EntityManager* g_entities;
    Entity* g_Player;
    int ids;    

    sf::RenderWindow g_window;
    sf::Font font;
    sf::Text score;
    sf::Text lives;
    sf::Text HighScore;
    sf::Text PressEnter;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::string Username = "JOHNDOE";
    std::string LivesText;
    bool Pause;
    int Lives = 0;
    cLifespan RespawnCooldown;
    cLifespan SpecialMoveCooldown;
    bool CoolDownActive = 0;
    bool isGameOver = 0;
    long int TotalScore = 0;
    vec2 ScreenSize;

    // **************** //

    Window window_vars;
    Font font_vars;
    Vars player_vars;
    Vars bullet_vars;
    Vars enemy_vars;

    bool fileinput(std::string);
    void SetLivesScore();
    void SpecialMove(int);
    void GameOver();
    void RestartGame();

public:
    Game();
    void sMovement();
    void sCollosion();
    void sUserInput();
    void sRender();
    void sEntitySpawner(const std::string, Vars&);
    void Play();
    ~Game();
};
Game :: Game():ids(0),Pause(0)
{ 
    std::cout << "Enter Username: ";
    std::cin >> Username;
    Game::fileinput(configPath);
    g_window.create(sf::VideoMode(window_vars.W,window_vars.H),"Shape Shooter");
    ScreenSize = g_window.getSize();

    g_entities = new EntityManager();
    g_window.setFramerateLimit(window_vars.FR);
    sEntitySpawner("player", player_vars);
    g_entities->update();
    g_Player = g_entities->getEntities("player")[0];
    RespawnCooldown.cooldown = RespawnCooldown.remaining = RespawnCooldownSeconds * window_vars.FR;
    SpecialMoveCooldown.cooldown = SpecialMoveCooldown.remaining
        = SpecialMoveCooldownSec * window_vars.FR;

    //score and lives fonts
    Game::SetLivesScore();
    
    backgroundTexture.loadFromFile(backgroundPath);
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u originalSize = backgroundTexture.getSize(); 
    sf::Vector2u targetSize(ScreenSize.x, ScreenSize.y);
    float scaleX = static_cast<float>(targetSize.x) / originalSize.x;
    float scaleY = static_cast<float>(targetSize.y) / originalSize.y;

    float scale = std::max(scaleX, scaleY); 
    backgroundSprite.setScale(scale, scale);
    backgroundSprite.setOrigin(backgroundSprite.getLocalBounds().width / 2, backgroundSprite.getLocalBounds().height / 2);
    backgroundSprite.setPosition(ScreenSize.x / 2, ScreenSize.y / 2);
    
    
}
Game :: ~Game()
{
    delete g_entities;
}

void Game::sMovement()
{
    
    if(g_Player->input->Left) g_Player->transform->position.x -= g_Player->transform->speed.x;
    if(g_Player->input->Right) g_Player->transform->position.x += g_Player->transform->speed.x;
    if(g_Player->input->Up) g_Player->transform->position.y -= g_Player->transform->speed.y;
    if(g_Player->input->Down) g_Player->transform->position.y += g_Player->transform->speed.y;
    if(g_Player->transform->position.x + g_Player->collosion->radius > ScreenSize.x)
    {
        g_Player->transform->position.x = ScreenSize.x - g_Player->collosion->radius;
    }
    else if(g_Player->transform->position.x - g_Player->collosion->radius < 0)
    {
        g_Player->transform->position.x = g_Player->collosion->radius;
    }
    if(g_Player->transform->position.y + g_Player->collosion->radius > ScreenSize.y)
    {
        g_Player->transform->position.y = ScreenSize.y - g_Player->collosion->radius;
    }
    else if(g_Player->transform->position.y - g_Player->collosion->radius < 0)
    {
        g_Player->transform->position.y = g_Player->collosion->radius;
    }

    g_Player->shape->shape.setPosition(g_Player->transform->position.x, g_Player->transform->position.y);
    g_Player->transform->angle += playerRotation;
    g_Player->shape->shape.setRotation(g_Player->transform->angle);

    for(auto& e:g_entities->getEntities("enemy"))
    {
        if(e->isActive())
        {
            if(e->lifespan->remaining > 0)
            {
                e->transform->position += e->transform->speed;
                e->shape->shape.setPosition(e->transform->position.x, e->transform->position.y);
                e->transform->angle += e->transform->speed.x;
                e->shape->shape.setRotation(e->transform->angle);
                e->lifespan->remaining--;
            }
            else
            {
                e->destroy();
            }
        }
    }

    for(auto& e:g_entities->getEntities("bullet"))
    {
        if(e->isActive())
        {
            if(e->lifespan->remaining > 0)
            {
                e->transform->position += e->transform->speed;
                e->shape->shape.setPosition(e->transform->position.x, e->transform->position.y);
                e->lifespan->remaining--;
            }
            else
            {
                e->destroy();
            }
        }
    }

    if(g_Player->input->M)
    {
        if (SpecialMoveCooldown.remaining == SpecialMoveCooldown.cooldown)
        {
            LivesText[0] = ' ';
            lives.setString(LivesText);
            SpecialMoveCooldown.remaining--;
            Game::SpecialMove(int(TotalScore/(ScorePerVertex*10) + MinSpecialMoveBullets) % maxSpecialMoveBullets);
        }
    }

    //if(g_Player->input->MouseClick)
    //  if(act < maxBullets)
        //Game::sEntitySpawner("bullet", bullet_vars);
}
void Game::sCollosion()
{
    // collosion with walls
    for(auto& e:g_entities->getEntities())
    {
        if(e->isActive())
        {
            if(e->transform->position.x + e->collosion->radius > ScreenSize.x || 
                e->transform->position.x - e->collosion->radius < 0)
            {
                    e->transform->speed.x *= -1;
            }
            if(e->transform->position.y + e->collosion->radius > ScreenSize.y || 
                e->transform->position.y - e->collosion->radius < 0)
            {
                    e->transform->speed.y *= -1;
            }
        }
    }
    
    float radius_sum;

    /*reduce cooldowns*/

    SpecialMoveCooldown.remaining -= SpecialMoveCooldown.remaining < SpecialMoveCooldown.cooldown;

    if(SpecialMoveCooldown.remaining <= 0)
    {
        LivesText[0] = '*';
        lives.setString(LivesText);
        SpecialMoveCooldown.remaining = SpecialMoveCooldown.cooldown;
    }
 
    if(RespawnCooldown.remaining > 0)
        RespawnCooldown.remaining--;
    else {
        CoolDownActive = 0;
        RespawnCooldown.remaining = RespawnCooldown.cooldown;
        sf::Color c = g_Player->shape->shape.getFillColor();
        c.a = 100;
        g_Player->shape->shape.setFillColor(c);
    }
    if(!CoolDownActive) {
    for(auto& e : g_entities->getEntities("enemy") )
    {
        if(e->isActive()
           && (e->lifespan->cooldown - e->lifespan->remaining > window_vars.FR /5) ) 
           // incase enemy spwns dirctly on player wait for 1/5th of sec
        {
            radius_sum = g_Player->collosion->radius + e->collosion->radius; 
            radius_sum *= radius_sum;
            // Player Collides with Enemy -- Destroy
            if( g_Player->transform->position.norm(e->transform->position) <= radius_sum)
            {
                /* Life decrese*/
                Lives++;
                LivesText.pop_back();
                LivesText[0] = SpecialMoveCooldown.remaining == SpecialMoveCooldown.cooldown? '*':' ';
                lives.setString(LivesText);
                CoolDownActive = 1;
                if(Lives >= TotalLives)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    isGameOver = 1;
                    GameOver();
                }
                g_Player->shape->shape.setPosition(ScreenSize.x /2 ,ScreenSize.y /2);
                g_Player->transform->position.x = ScreenSize.x  /2;
                g_Player->transform->position.y = ScreenSize.y /2;
                RespawnCooldown.remaining = RespawnCooldown.cooldown;
                sf::Color c = g_Player->shape->shape.getFillColor();
                c.a = 10;
                g_Player->shape->shape.setFillColor(c);
                break;
                
            }
        }
    }}
    
    for(auto& b: g_entities->getEntities("bullet"))
    {
        if(b->isActive())
        {
            vec2 bullet_position = b->transform->position;
            for (auto& e : g_entities->getEntities("enemy"))
            {
                if(e->isActive())
                {
                    radius_sum = b->collosion->radius + e->collosion->radius; 
                    radius_sum *= radius_sum;
                    if( bullet_position.norm(e->transform->position) <= radius_sum)
                    {
                        /*make smaller enemies*/
                        TotalScore += e->score->score;
                        score.setString("Score: "+std::to_string(TotalScore));
                        e->destroy();
                         
                    }
                }
            }
        }
    }
}
void Game::sRender()
{
    g_window.clear();
    g_window.draw(backgroundSprite);
    g_window.draw(score);
    g_window.draw(lives);

    if(isGameOver)
    {
        g_window.draw(HighScore);
        g_window.draw(PressEnter);
        g_window.display();
        return;
    }

    for(auto& e:g_entities->getEntities())
    {
        if(e->isActive())
        {
            g_window.draw(e->shape->shape);
        }
    }
    g_window.display();
}
void Game:: sUserInput()
{
    sf::Event event;
    g_Player->input->flush();
    while (g_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            g_window.close();
        else if (event.type == sf::Event::KeyPressed)
        {
            if(isGameOver)
            {
                if(event.key.code == sf::Keyboard::Enter)
                {
                    Game::RestartGame();
                }
            }
            if (event.key.code == sf::Keyboard::Escape)
            {
                Pause = !Pause;
                continue;
            }
            g_Player->input->Left = event.key.code == sf::Keyboard::Left;
            g_Player->input->Right = event.key.code == sf::Keyboard::Right; 
            g_Player->input->Up = event.key.code == sf::Keyboard::Up;
            g_Player->input->Down = event.key.code == sf::Keyboard::Down;
            g_Player->input->M = event.key.code == sf::Keyboard::Space;
        }   
        g_Player->input->MouseClick = event.type == sf::Event::MouseButtonPressed;
        if (g_Player->input->MouseClick)
        {
            g_Player->input->MousePosition.x = event.mouseButton.x;
            g_Player->input->MousePosition.y = event.mouseButton.y;
            /*spawn a bullet*/
            
            if(g_entities->getEntities("bullet").size() < maxBullets)
                Game::sEntitySpawner("bullet", bullet_vars);
        }
    }            
}

void Game::sEntitySpawner(const std::string tag, Vars& parameters)
{

    Entity* Temp_Entity = new Entity(tag, ids++, 1);
    int vertices{};
    int random = rand();
    vec2 speed {parameters.S,parameters.S};
    sf::Color ForeBackground = sf::Color(parameters.FR,parameters.FG,parameters.FB);
    vec2 position;
    if(tag == "enemy")
    {
        vertices = random % parameters.VMAX + parameters.VMIN;
        Temp_Entity->score = new cScore(ScorePerVertex*vertices);
        position.x = random % int(ScreenSize.x-int(parameters.CR *2))+parameters.CR;
        position.y = random % int(ScreenSize.y-int(parameters.CR *2))+parameters.CR;
        Temp_Entity->lifespan = new cLifespan(parameters.L);
        speed.x = rand() % int(parameters.SMAX) + parameters.SMIN;
        speed.y = rand() % int(parameters.SMAX) + parameters.SMIN;
    }
    else
    {
        vertices = parameters.V;
        if(tag == "bullet")
        {
            Temp_Entity->lifespan = new cLifespan(parameters.L);
            vec2 PlayerCenter = g_Player->shape->shape.getPosition();
            float theta = atan((g_Player->input->MousePosition.y - PlayerCenter.y) / (g_Player->input->MousePosition.x - PlayerCenter.x));
            speed.x = parameters.S* cos(theta);
            speed.y = parameters.S* sin(theta);
            position.x = g_Player->transform->position.x + parameters.CR* cos(theta);
            position.y = g_Player->transform->position.y + parameters.CR* sin(theta);
            if(g_Player->input->MousePosition.x < PlayerCenter.x) 
            {
                speed.x *=-1;
                speed.y *=-1;
            }
        }
        else if (tag == "player")
        {
            position.x = (ScreenSize.x /2);
            position.y = (ScreenSize.y /2);
            Temp_Entity->input = new cInput();
        }
    }

    Temp_Entity->shape = new cShape(ForeBackground,
        sf::Color(parameters.OR,parameters.OG,parameters.OB), position, parameters.OT,
        vertices, parameters.SR);

    Temp_Entity->transform = new cTransform(speed,position);
    Temp_Entity->collosion = new cCollision(parameters.CR);

    g_entities->addEntity(Temp_Entity);


}

void Game::GameOver()
{
    g_window.setFramerateLimit(window_vars.FR/3);   //Decrease FR To reduce computations

    /*save users score in file*/
    
    std::ofstream outFile(scoreFilePath, std::ios::app);
    //std::time_t now = std::time(nullptr);  
    outFile << Username << " " << TotalScore << "\n";
    outFile.close();  

    /*Now find highest score from filr and display it*/

    std::string TopScorers;
    int maxScore = -1;
    std::string currUsername;
    int currScore = 0;

    std::ifstream infile(scoreFilePath);

    while(infile >> currUsername >> currScore)
    {
        if(currScore > maxScore )
        {
            TopScorers.clear();
            TopScorers = currUsername;
            maxScore = currScore;
        }
        else if (currScore == maxScore)
        {
            TopScorers += "-" + currUsername;
        }
    }
    infile.close();

    

    int game_over_char_size = 50;

    lives.setString("Game Over");
    lives.setFillColor(sf::Color::Red);
    lives.setCharacterSize(game_over_char_size);
    lives.setPosition(sf::Vector2f(ScreenSize.x/2 - lives.getGlobalBounds().width/2, 
    GameOverTextYPosition));


    score.setCharacterSize(game_over_char_size*0.4);
    score.setString(score.getString());
    score.setPosition(sf::Vector2f(ScreenSize.x/2 - score.getGlobalBounds().width/2,  
    lives.getPosition().y + lives.getCharacterSize()*1.5));

    HighScore = score;
    HighScore.setString("High Score: " + TopScorers + " : " + std::to_string(maxScore));
    HighScore.setPosition(sf::Vector2f(ScreenSize.x/2 - HighScore.getGlobalBounds().width/2,  
    score.getPosition().y + score.getCharacterSize()*1.5));

    PressEnter = HighScore;
    PressEnter.setString("Press Enter to Play Again");
    PressEnter.setFillColor(sf::Color::Red);
    PressEnter.setPosition(sf::Vector2f(ScreenSize.x/2 - PressEnter.getGlobalBounds().width/2,  
    HighScore.getPosition().y + HighScore.getCharacterSize()*1.5));
    

}

void Game::RestartGame()
{
    TotalScore = 0;
    Lives = 0;
    g_window.setFramerateLimit(window_vars.FR);
    isGameOver = 0;
    delete g_entities;     
    g_entities = new EntityManager();
    sEntitySpawner("player", player_vars);
    g_entities->update();
    g_Player = g_entities->getEntities("player")[0];
    Game::SetLivesScore();
}

void Game::SetLivesScore()
{
    font.loadFromFile(fontPath);
    score.setFont(font);
    score.setFillColor(sf::Color::Blue);
    score.setStyle(sf::Text::Bold);
    score.setCharacterSize(TextSize);
    score.setPosition(ScreenSize.x-200, ScreenSize.y-TextSize-10);
    score.setString("Score: 0");
    lives = score;
    LivesText = "* Lives: ";
    LivesText.reserve(9 + TotalLives);
    
    for (int i = 0; i < TotalLives; i++)
        LivesText += "+";

    lives.setFillColor(sf::Color::Red);
    lives.setPosition(200, ScreenSize.y-TextSize-10);
    lives.setString(LivesText);
}

void Game::SpecialMove(int totalBulletsSpawn)
{
    /*spawn wall of bullets*/

    double angle_increase = (2 * M_PI ) / totalBulletsSpawn;
    double angle = 0;

    for (int i = 0; i < totalBulletsSpawn; i++)
    {
        g_Player->input->MousePosition.x = g_Player->shape->shape.getPosition().x + g_Player->collosion->radius * cos(angle);
        g_Player->input->MousePosition.y = g_Player->shape->shape.getPosition().y - g_Player->collosion->radius * sin(angle);
        angle += angle_increase;
        Game::sEntitySpawner("bullet", bullet_vars);
    }
}


bool Game :: fileinput (std::string filename)
{
    
    std::ifstream file(filename);
    if(!file)                               // create config file if not found
    {
        std::ofstream outfile(filename);
        outfile << configerations;
        outfile.close();
        file.open(filename);
    }
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "Window") {
                iss >> window_vars.W >> window_vars.H >> window_vars.FR >> window_vars.FS;
            } else if (token == "Font") {
                iss >> font_vars.F >> font_vars.S >> font_vars.R >> font_vars.G >> font_vars.B;
            } else if (token == "Player") {
                iss >> player_vars.SR >> player_vars.CR >> player_vars.S >> player_vars.FR >> player_vars.FG >> player_vars.FB
                    >> player_vars.OR >> player_vars.OG >> player_vars.OB >> player_vars.OT >> player_vars.V;
                player_vars.CR = player_vars.SR+player_vars.OT;
            } else if (token == "Enemy") {
                iss >> enemy_vars.SR >> enemy_vars.CR >> enemy_vars.SMIN >> enemy_vars.SMAX >> enemy_vars.FR >> enemy_vars.FG >> enemy_vars.FB
                    >> enemy_vars.OR >> enemy_vars.OG >> enemy_vars.OB >> enemy_vars.OT >> enemy_vars.VMIN >> enemy_vars.VMAX >> enemy_vars.L >> enemy_vars.SP;
                    enemy_vars.CR = enemy_vars.SR+enemy_vars.OT;
            } else if (token == "Bullet") {
                iss >> bullet_vars.SR >> bullet_vars.CR >> bullet_vars.S >> bullet_vars.FR >> bullet_vars.FG >> bullet_vars.FB
                    >> bullet_vars.OR >> bullet_vars.OG >> bullet_vars.OB >> bullet_vars.OT >> bullet_vars.V >> bullet_vars.L;
                bullet_vars.CR = bullet_vars.SR+bullet_vars.OT;
            }
            
        }
        file.close();

    } 
    return true;
}



void Game :: Play()
{
   
    srand(time(0));
    int frames = enemy_vars.SP /2;
    while(g_window.isOpen())
    {
        sUserInput();
        if(isGameOver)
            sRender();
        else if(!Pause) 
        {
            g_entities->update();
            if(frames++ >= enemy_vars.SP)
            { 
                sEntitySpawner("enemy", enemy_vars);
                frames = 0;
            }
            sCollosion();
            sMovement();
            sRender();
        }
    }
}


int main()
{
    Game ShapeShooter;
    ShapeShooter.Play();
}
