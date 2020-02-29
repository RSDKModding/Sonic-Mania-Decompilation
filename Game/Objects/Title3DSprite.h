#ifndef OBJ_TITLE3DSPRITE_H
#define OBJ_TITLE3DSPRITE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTitle3DSprite : public Object {
public:

};

//Entity Class
class EntityTitle3DSprite : public Entity {
public:

};

//Object Entity
ObjectTitle3DSprite Title3DSprite;

//Entity Functions
void Title3DSprite_Update();
void Title3DSprite_EarlyUpdate();
void Title3DSprite_LateUpdate();
void Title3DSprite_Draw();
void Title3DSprite_Setup(void* subtype);
void Title3DSprite_StageLoad();
void Title3DSprite_GetAttributes();

#endif //!OBJ_TITLE3DSPRITE_H
