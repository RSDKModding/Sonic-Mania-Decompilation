#ifndef OBJ_PARALLAXSPRITE_H
#define OBJ_PARALLAXSPRITE_H

//Object Class
class ParallaxSprite : Object {

};

//Entity Class
class EntityParallaxSprite : Entity {

};

//Entity Functions
void ParallaxSprite_Update();
void ParallaxSprite_EarlyUpdate();
void ParallaxSprite_LateUpdate();
void ParallaxSprite_Draw();
void ParallaxSprite_Setup(void* subtype);
void ParallaxSprite_StageLoad();
void ParallaxSprite_GetAttributes();

#endif //!OBJ_PARALLAXSPRITE_H
