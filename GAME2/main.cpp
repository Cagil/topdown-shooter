
// Easy way to include libraries!
#pragma comment(lib, "opengl32.lib")

/*	nvImage.h contains all of the function prototypes needed for image loading.
	This MUST be included before gl.h and glu.h because of dependencies therein - WAYNE! */
#include "Image_Loading/nvImage.h"

#include <fstream>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <math.h>
#include <vector>
#include <sstream>

#include "console.h"		// Header file for Console

#include "Object.h"
#include "Map.h"
#include "Player.h"
#include "BoundaryLine.h"
#include "BBox.h"
#include "HUDbar.h"
#include "Zombie.h"
#include "Point.h"
#include "Powerup.h"

#include "glfont2.h"
using namespace glfont;

//glFont
GLFont myfont;
ConsoleWindow console;

int	mouse_x=0, mouse_y=0;
bool LeftPressed,RightPressed = false;
bool pressed = false;
int screenWidth=480, screenHeight=480;
bool keys[256];

bool gameover = false;
bool won = false;

Player player;
Bullet temp_bullet;
Map map1(800,960);

//Zombie initialisations
Zombie z;
int summoned_zombies = 0;
int MAX_ZOMBIE_NUMBER = 16;
int spawn_order = 0;
Point* spawn_points[4];

Point zombie_spawn_1 ;
Point zombie_spawn_2 ;
Point zombie_spawn_3 ;
Point zombie_spawn_4 ;

std::vector<Zombie> zombies;
double zombieSpawnTimer;
bool canSpawnZombie = false;
//Zombie END

//POWER UP initialisations
int noSpawnPowerup = false;
int pup_spawn_order = 0;
double powerupSpawnTimer;
Point powerup_spawn_1;
Point powerup_spawn_2;
Point powerup_spawn_3;
Point powerup_spawn_4;
std::vector<Powerup> powerups;

Point* powerup_spawn_points[4];
//POWERUP END

//User Interface
HUDbar healthbar = HUDbar(0,8,240,15,HUDbar::HEALTH);
HUDbar expbar = HUDbar(0,0,480,8,HUDbar::EXPERIENCE);


//texture ints for objects
GLuint tt = 0; 
GLuint heart_tex = 0;
GLuint ammo_tex = 0;
GLuint bullet_tex = 0;
GLuint bg_tex = 0;
GLuint zombie_tex = 0;
GLuint game_over_tex = 0;




//OPENGL FUNCTION PROTOTYPES
void display(double);				//draws everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys(double);			//called in winmain to process keyboard controls


string convertFloat(float);
string convertInt(int);
/*************    START OF OPENGL FUNCTIONS   ****************/
void display(double delta)									
{	
	if(!gameover){
	glClear(GL_COLOR_BUFFER_BIT);

	//CAMERA following player, player at the center of the screen all the time.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float x, x2,y,y2;
	x = player.getX() - 240;
	x2 = player.getX() + 240;
	y = player.getY() - 240;
	y2 = player.getY() + 240;
	gluOrtho2D( x,x2,y,y2);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	///////////////////////////CAMERA ENDS///////////////////////////////////

	//DRAWS MAP [, LINES & BBOXES] ( if '.drawLines' is set true )	
	map1.draw();
	
	//draws updates power ups
	for(int i = 0;i<powerups.size();i++){
		
		powerups.at(i).draw();
		powerups.at(i).update(delta, &player,&powerups);

		//if any one of powerups is taken rest gets removed 
		//then starts the powerup start timer again. 
		// if set true no power ups will ever be created.
		if(powerups.size() == 0)
			noSpawnPowerup = false;
	}

	//handles zombies, draws updates, checks for collisions
	for(int i = 0;i<zombies.size();i++){
		if(zombies.at(i).isAlive()){
			zombies.at(i).update(delta, player.getX(), player.getY(), &map1.getBBoxes());
			zombies.at(i).draw();	
			
			//if collides with the player, stops and then attacks
			// if not moves faster.
			if(!zombies.at(i).checkPlayerCollision(&player)){							
				zombies.at(i).setMultiplier(1.5);
			}else{			
				zombies.at(i).attack(&player);
			}

			//if the zombie is on a yellow spots zombies movie faster
			if(map1.getTile(zombies.at(i).getX(),zombies.at(i).getY()) == 1)
				zombies.at(i).setMultiplier(2.05);
			
			//if(zombies.at(i).isOutOfTrack(&map1.getTrackLines()))
			//	zombies.at(i).setMultiplier(2.5);
			
		}else		
			continue;
	}

	
				
	//checks if player is on the track or outside, slows down if not or moves normal speed if on the track
	player.isOutOfTrack(&map1.getTrackLines());
	//updates player position and angle
	player.update(delta,mouse_x,mouse_y);

	//draws player texture
	player.draw();
	
	//zombie spawn timer
	if(!canSpawnZombie){
		zombieSpawnTimer += delta;	
	}


	// 300000 is th usual time, but for testing 200000
	if(zombieSpawnTimer > 200000 && summoned_zombies < MAX_ZOMBIE_NUMBER){		
		//create zombie at one of the spawn points
		Zombie z = Zombie(32,32,spawn_points[spawn_order]->getX(),spawn_points[spawn_order]->getY(),zombie_tex);
		//set zombie attributes according to players rank
		z.setAttributes(player.getRank());
		//add zombie to the zombie list
		zombies.push_back(z);
	
		summoned_zombies++;
		//change to next spawn point
		spawn_order++;

		//resett te spawn point
		if(spawn_order > 3)
			spawn_order = 0;
		//reset the timer
		zombieSpawnTimer = 0;
	}


	//powerup spawnn timer
	if(!noSpawnPowerup){
		powerupSpawnTimer += delta;
	}

	if(powerupSpawnTimer >= 500000){
		

		//creates the powerup at one of the spawn points
		Powerup pupHP = Powerup(powerup_spawn_points[pup_spawn_order]->getX(),powerup_spawn_points[pup_spawn_order]->getY(),16,16,heart_tex,Powerup::HP);	
		//change to next spawn point
		pup_spawn_order++;

		//if reached the last spawn point resett the order
		if(pup_spawn_order > 3)
			pup_spawn_order = 0;
		
		//create the second powerup at the next spawn point
		Powerup pupAMMO = Powerup(powerup_spawn_points[pup_spawn_order]->getX(),powerup_spawn_points[pup_spawn_order]->getY(),16,16,ammo_tex,Powerup::AMMO);	
		
		//add powerups to the list
		powerups.push_back(pupAMMO);
		powerups.push_back(pupHP);

		
		noSpawnPowerup = true;
		//reset the timer
		powerupSpawnTimer = 0;
	}
	
	//collision detection for bullets with zombies , other objects in game.
	player.checkBulletCollision(&zombies,&map1.getBBoxes());

	/////////////////////////////HUD/////////////////////////
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,screenWidth,0,screenHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

		//TEXT-HUD
		glPushMatrix();
			glPushMatrix();
				glBegin(GL_QUADS);
				glColor3f(0.4f,0.4f,0.4f);
					glVertex2f(0,0);
					glVertex2f(0,50);
					glVertex2f(screenWidth, 50);
					glVertex2f(screenWidth, 0);
				glEnd();
			glPopMatrix();

			//updates, draws health bar
			healthbar.update(&player);
			healthbar.draw();

			//updates draw experience bar
			expbar.update(&player);
			expbar.draw();


			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);

			float top_color[3] ={0.0,0.0,0.0};
			float bottom_color[3] = {0.0,0.0,0.0};

			myfont.Begin();
			string hp = "HP: "; 
			hp += convertInt(player.getHealth());
			hp += "/";
			hp += convertInt(player.getMaxHealth());
			
			string ammo = "Ammo: ";
			ammo += convertInt(player.getCurrentClipSize());
			ammo += " (";
			ammo += convertInt(player.getTotalAmmo());
			ammo += ")";

			string rank = "Rank: ";
			rank += convertInt(player.getRank());

			string dmg = "Damage: ";
			dmg += convertInt(player.getDamage());

			//draws text HUDs
			myfont.DrawString(hp, 0.6f, 55.0f, 27.0f, top_color, bottom_color);
			myfont.DrawString(rank, 0.6f, 0, 50.0f, top_color, bottom_color);
			myfont.DrawString(ammo, 0.6f, 320, 29.0f, top_color, bottom_color);
			myfont.DrawString(dmg, 0.6f, 320, 50.0f, top_color, bottom_color);
		
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glColor3f(1,1,1);
		glPopMatrix();

	glPopMatrix();	
	
	/////////////////////HUD ENDS/////////////////////////////
	}
	else{
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, game_over_tex);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0,screenHeight);
				glTexCoord2f(1.0, 1.0); glVertex2f(screenWidth,screenHeight);
				glTexCoord2f(1.0, 0.0); glVertex2f(screenWidth,0);
			glEnd();
		
		glPopMatrix();

		float top_color[3] ={1.0,1.0,1.0};
		float bottom_color[3] = {1.0,1.0,1.0};

		myfont.Begin();

		string msg;
		if(won){
			msg = "YOU WON";
		}
		else{
			msg = "YOU LOST";
			string score ="YOU'VE KILLED ";
			score += convertInt(player.getKillCount());
			score += " ZOMBIES!";

			myfont.DrawString(score, 0.6f, 115.0, 275.0f, top_color, bottom_color);
		}

		myfont.DrawString(msg, 0.6f, 190.0f, 300.0f, top_color, bottom_color);
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}
	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width;											 // to ensure the mouse coordinates match 
	screenHeight=height;									// we will use these values to set the coordinate system
	glViewport(0,0,(GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glLoadIdentity();											// Reset The Projection Matrix		
	gluOrtho2D(0,screenWidth,0,screenHeight);					// set the coordinate system for the window	
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
}
void init()
{
	glClearColor(0.0,0.0,0.0,0.0);	 //sets the clear colour to black
	
	map1.setupTrackLines();
	map1.setupBoundingBoxes();

	//CHANGE IF WANTED TO SEE BOUNDING BOXES AND TRACK LINES
	//DEFAULT all false
	map1.drawLines = false;	
	Player::drawLines = false;
	Zombie::drawLines = false;
	///////////////////////////////////////////////////////////

	temp_bullet.MAX_X_BOUND = map1.getWidth();
	temp_bullet.MAX_Y_BOUND = map1.getHeight();

	zombie_spawn_1 = Point(32.0f,150.0f);
	zombie_spawn_2 = Point(800.0f,150.0f);
	zombie_spawn_3 = Point(32.0f,728.0f);
	zombie_spawn_4 = Point(800.0f,725.0f);

	spawn_points[0] = &zombie_spawn_1;
	spawn_points[1] = &zombie_spawn_2;
	spawn_points[2] = &zombie_spawn_3;
	spawn_points[3] = &zombie_spawn_4;

	powerup_spawn_1 = Point(206.0,223.0);	
	powerup_spawn_2 = Point(736.0,512.0);
	powerup_spawn_3 = Point(625.0, 75.0);
	powerup_spawn_4 = Point(207.0, 724.0);

	powerup_spawn_points[0] = &powerup_spawn_1;
	powerup_spawn_points[1] = &powerup_spawn_2;
	powerup_spawn_points[2] = &powerup_spawn_3;
	powerup_spawn_points[3] = &powerup_spawn_4;

	// Texture loading object
	nv::Image img;

	// background texture loading
	if(img.loadImageFromFile("Assets/bg.png"))
	{
		glGenTextures(1, &bg_tex);
		glBindTexture(GL_TEXTURE_2D, bg_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		
		map1.setBgTexture(bg_tex);	
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	//health powerup texture loading
	nv::Image heart;			
	if(heart.loadImageFromFile("Assets/heart.png"))
	{
		glGenTextures(1, &heart_tex);
		glBindTexture(GL_TEXTURE_2D, heart_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, heart.getInternalFormat(), heart.getWidth(), heart.getHeight(), 0, heart.getFormat(), heart.getType(), heart.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		
	}
	else
		MessageBox(NULL, "Failed to load texturebg", "End of the world", MB_OK | MB_ICONINFORMATION);

	//zombie texture loading
	nv::Image zombie;
	if(zombie.loadImageFromFile("Assets/zombie.png"))
	{
		glGenTextures(1, &zombie_tex);
		glBindTexture(GL_TEXTURE_2D, zombie_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, zombie.getInternalFormat(), zombie.getWidth(), zombie.getHeight(), 0, zombie.getFormat(), zombie.getType(), zombie.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);		
	}
	else
		MessageBox(NULL, "Failed to load texturetra", "End of the world", MB_OK | MB_ICONINFORMATION);
	
	//ammo powerup texture loading
	nv::Image img1;
	if(img1.loadImageFromFile("Assets/ammo.png"))
	{
		glGenTextures(1, &ammo_tex);
		glBindTexture(GL_TEXTURE_2D, ammo_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img1.getInternalFormat(), img1.getWidth(), img1.getHeight(), 0, img1.getFormat(), img1.getType(), img1.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	//bullet texture loading
	nv::Image img2;
	if(img2.loadImageFromFile("Assets/bullet.png"))
	{
		glGenTextures(1, &bullet_tex);
		glBindTexture(GL_TEXTURE_2D, bullet_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img2.getInternalFormat(), img2.getWidth(), img2.getHeight(), 0, img2.getFormat(), img2.getType(), img2.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

		player.setBulletTexture(bullet_tex);	
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	//player texture loading
	nv::Image img3;
	if(img3.loadImageFromFile("Assets/player.png"))
	{
		glGenTextures(1, &tt);
		glBindTexture(GL_TEXTURE_2D, tt);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img3.getInternalFormat(), img3.getWidth(), img3.getHeight(), 0, img3.getFormat(), img3.getType(), img3.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		
		player = Player(img3.getWidth(),img3.getHeight(),416,64);
		player.setTexture(tt);
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

		//game over screen texture loading
	nv::Image img4;
	if(img4.loadImageFromFile("Assets/gameover.png"))
	{
		glGenTextures(1, &game_over_tex);
		glBindTexture(GL_TEXTURE_2D, game_over_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img4.getInternalFormat(), img4.getWidth(), img4.getHeight(), 0, img4.getFormat(), img4.getType(), img4.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	
	}
	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Create our glFont from Arial.glf, using texture 13
	if (!myfont.Create("Arial.glf", 13))
		exit(0);		
}
void processKeys(double delta)
{
	if(keys['W'])
	{
		pressed = true;
		if(player.getY() + 32 < map1.getHeight()){		
			player.checkCollision(map1.getBBoxes(),delta,'U');
		}
		
	}

	

	if(keys['A'])
	{
		pressed = true;
		if(player.getX() - 32 > 0){							
			player.checkCollision(map1.getBBoxes(),delta,'L');
		}				
	}

	if(keys['S'])
	{
		if(player.getY() - 32 > 0){
				player.checkCollision(map1.getBBoxes(),delta,'D');			
		}
	}


	if(keys['D'])
	{		
		if(player.getX() + 32 < map1.getWidth()){
			player.checkCollision(map1.getBBoxes(),delta,'R');
		}
	}

	if(keys['R']){
		player.reload();

		cout<<"total: "<<player.getTotalAmmo()<<endl;
		cout<<"current: "<<player.getCurrentClipSize()<<endl;
	}

	if(LeftPressed)
	{
			player.fire(mouse_x, mouse_y);	
		
	}

	if(RightPressed)
		player.setPointToMove(mouse_x,mouse_y);
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function


//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	LARGE_INTEGER freq, last, current;
	double delta = 0.0f;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&last);
	
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop


	console.Open();

	// Create Our OpenGL Window
	if (!CreateGLWindow("ZOMG! ZOMBIES!",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	
	
	while(!done)									// Loop That Runs While done=FALSE
	{
		QueryPerformanceCounter(&current);	
		
		delta = (double)(current.QuadPart-last.QuadPart) /50;
		
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;
		
			if(player.getKillCount() == MAX_ZOMBIE_NUMBER){
				gameover = true;
				won = true;
			}

			if(player.getHealth() <= 0){
				gameover= true;
				won = false;
			}

			processKeys(delta);
			display(delta);					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)

			last = current;
		}
	}
	

	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_RBUTTONDOWN:
			{
	           // mouse_x = LOWORD(lParam);          
				//mouse_y = screenHeight - HIWORD(lParam);
				RightPressed = true;
			}
		break;

		case WM_RBUTTONUP:
			{
	            RightPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);//
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
					
	dwStyle=(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);// Windows Style //no resizing no maximizing		
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}


//converters for text HUD
string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

string convertFloat(float number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}