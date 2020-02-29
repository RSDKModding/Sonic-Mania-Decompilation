#ifndef OBJ_AIZKINGCLAW_H
#define OBJ_AIZKINGCLAW_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZKingClaw : public Object {
public:

};

//Entity Class
class EntityAIZKingClaw : public Entity {
public:

};

//Object Entity
ObjectAIZKingClaw AIZKingClaw;

//Entity Functions
void AIZKingClaw_Update();
void AIZKingClaw_EarlyUpdate();
void AIZKingClaw_LateUpdate();
void AIZKingClaw_Draw();
void AIZKingClaw_Setup(void* subtype);
void AIZKingClaw_StageLoad();
void AIZKingClaw_GetAttributes();

#endif //!OBJ_AIZKINGCLAW_H
