#ifndef OBJ_TITLE3DSPRITE_H
#define OBJ_TITLE3DSPRITE_H

//Object Class
class Title3DSprite : Object {

};

//Entity Class
class EntityTitle3DSprite : Entity {

};

//Entity Functions
void Title3DSprite_Update();
void Title3DSprite_EarlyUpdate();
void Title3DSprite_LateUpdate();
void Title3DSprite_Draw();
void Title3DSprite_Setup(void* subtype);
void Title3DSprite_StageLoad();
void Title3DSprite_GetAttributes();

#endif //!OBJ_TITLE3DSPRITE_H
