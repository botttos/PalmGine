#ifndef GLOBALS_H
#define GLOBALS_H

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define BINARY_MESH_PATH "Library/Meshes/"
#define BINARY_TEXTURES_PATH "Library/Textures/"
#define MODELS_PATH "Models/"
#define TEXTURES_PATH "Textures/"


#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// ~~ FUNCTIONS ~~
bool FileExist(const std::string & name);
// Configuration -----------
//#define SCREEN_WIDTH 1280
//#define SCREEN_HEIGHT 1024
//#define SCREEN_SIZE 1
//#define WIN_FULLSCREEN false
//#define WIN_RESIZABLE true
//#define WIN_BORDERLESS false
//#define WIN_FULLSCREEN_DESKTOP false
//#define VSYNC true
//#define TITLE "PalmGine 3D"

#endif // !GLOBALS_H