#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectEncoreIntro : public Object {
public:

};

//Entity Class
class EntityEncoreIntro : public Entity {
public:

};

//Object Entity
ObjectEncoreIntro EncoreIntro;

//Entity Functions
void EncoreIntro_Update();
void EncoreIntro_EarlyUpdate();
void EncoreIntro_LateUpdate();
void EncoreIntro_Draw();
void EncoreIntro_Setup(void* subtype);
void EncoreIntro_StageLoad();
void EncoreIntro_GetAttributes();

#endif //!OBJ_ENCOREINTRO_H
