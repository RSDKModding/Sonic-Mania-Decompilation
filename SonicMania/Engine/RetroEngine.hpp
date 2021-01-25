#ifndef RETROENGINE_H
#define RETROENGINE_H

// Disables POSIX use c++ name blah blah stuff
#pragma warning(disable : 4996)

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>
#include <cmath>

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;


// Utils
#include "iniparser/iniparser.h"

#include "Math.hpp"
//#include "Reader.hpp"
//#include "Animation.hpp"
//#include "Audio.hpp"
//#include "Input.hpp"
#include "Object.hpp"
//#include "Palette.hpp"
#include "Drawing.hpp"
//#include "Scene3D.hpp"
#include "Collision.hpp"
//#include "Scene.hpp"
#include "Sprite.hpp"
//#include "Text.hpp"
//#include "Userdata.hpp"
//#include "Debug.hpp"

class RetroEngine
{
public:
    RetroEngine() {}

    void init();
    void run();
};

extern RetroEngine engine;

#endif //! RETROENGINE_H
