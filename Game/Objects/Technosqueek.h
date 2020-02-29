#ifndef OBJ_TECHNOSQUEEK_H
#define OBJ_TECHNOSQUEEK_H

#include "../../SonicMania.h"

//Object Class
class ObjectTechnosqueek : public Object {
public:

};

//Entity Class
class EntityTechnosqueek : public Entity {
public:

};

//Object Entity
ObjectTechnosqueek Technosqueek;

//Entity Functions
void Technosqueek_Update();
void Technosqueek_EarlyUpdate();
void Technosqueek_LateUpdate();
void Technosqueek_Draw();
void Technosqueek_Setup(void* subtype);
void Technosqueek_StageLoad();
void Technosqueek_GetAttributes();

#endif //!OBJ_TECHNOSQUEEK_H
