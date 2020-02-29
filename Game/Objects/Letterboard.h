#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectLetterboard : public Object {
public:

};

//Entity Class
class EntityLetterboard : public Entity {
public:

};

//Object Entity
ObjectLetterboard Letterboard;

//Entity Functions
void Letterboard_Update();
void Letterboard_EarlyUpdate();
void Letterboard_LateUpdate();
void Letterboard_Draw();
void Letterboard_Setup(void* subtype);
void Letterboard_StageLoad();
void Letterboard_GetAttributes();

#endif //!OBJ_LETTERBOARD_H
