******* Install  ********

Run ./installer.sh on linux terminal to install the game
Run ./uninstaller.sh on linux terminal to uninstall the game

***** Playing Instructions *****

    Press Escape to Pause.
    Use the Arrow Keys to move the Player.
    Press Space to use the Special Move.
    An asterisk (*) behind "Lives" indicates the availability of the Special Move.
    Edit config.txt to control graphics settings.
    After a life is lost, the Player is invincible until its opacity fully increases.
    


<<<<<<<< Config file Structure >>>>>>>>

Window 1280 720 60 0
Player 32 32 5 5 5 5 255 0 0 7
Enemy 32 32 3 3 255 255 255 2 3 8 90 60
Bullet 10 10 20 255 255 255 255 255 255 2 20 90



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

Note: If you intend to compile and run the binary file, run installer first as the binary file loads data files after installer copies them to the intended path
