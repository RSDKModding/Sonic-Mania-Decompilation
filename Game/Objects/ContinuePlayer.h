#ifndef OBJ_CONTINUEPLAYER_H
#define OBJ_CONTINUEPLAYER_H

#include "../../SonicMania.h"

//Object Class
class ObjectContinuePlayer : public Object {
public:

};

//Entity Class
class EntityContinuePlayer : public Entity {
public:

};

//Object Entity
ObjectContinuePlayer ContinuePlayer;

//Entity Functions
void ContinuePlayer_Update();
void ContinuePlayer_EarlyUpdate();
void ContinuePlayer_LateUpdate();
void ContinuePlayer_Draw();
void ContinuePlayer_Setup(void* subtype);
void ContinuePlayer_StageLoad();
void ContinuePlayer_GetAttributes();

#endif //!OBJ_CONTINUEPLAYER_H
