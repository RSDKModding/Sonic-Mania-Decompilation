#ifndef OBJ_PARALLAXSPRITE_H
#define OBJ_PARALLAXSPRITE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectParallaxSprite : Object {

};

// Entity Class
struct EntityParallaxSprite : Entity {

};

// Object Struct
extern ObjectParallaxSprite *ParallaxSprite;

// Standard Entity Events
void ParallaxSprite_Update();
void ParallaxSprite_LateUpdate();
void ParallaxSprite_StaticUpdate();
void ParallaxSprite_Draw();
void ParallaxSprite_Create(void* data);
void ParallaxSprite_StageLoad();
void ParallaxSprite_EditorDraw();
void ParallaxSprite_EditorLoad();
void ParallaxSprite_Serialize();

// Extra Entity Functions


#endif //!OBJ_PARALLAXSPRITE_H
