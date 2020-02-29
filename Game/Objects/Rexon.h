#ifndef OBJ_REXON_H
#define OBJ_REXON_H

#include "../../SonicMania.h"

//Object Class
class ObjectRexon : public Object {
public:

};

//Entity Class
class EntityRexon : public Entity {
public:

};

//Object Entity
ObjectRexon Rexon;

//Entity Functions
void Rexon_Update();
void Rexon_EarlyUpdate();
void Rexon_LateUpdate();
void Rexon_Draw();
void Rexon_Setup(void* subtype);
void Rexon_StageLoad();
void Rexon_GetAttributes();

#endif //!OBJ_REXON_H
