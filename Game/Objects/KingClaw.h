#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

#include "../../SonicMania.h"

//Object Class
class ObjectKingClaw : public Object {
public:

};

//Entity Class
class EntityKingClaw : public Entity {
public:

};

//Object Entity
ObjectKingClaw KingClaw;

//Entity Functions
void KingClaw_Update();
void KingClaw_EarlyUpdate();
void KingClaw_LateUpdate();
void KingClaw_Draw();
void KingClaw_Setup(void* subtype);
void KingClaw_StageLoad();
void KingClaw_GetAttributes();

#endif //!OBJ_KINGCLAW_H
