#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

//Object Class
class KingClaw : Object {

};

//Entity Class
class EntityKingClaw : Entity {

};

//Entity Functions
void KingClaw_Update();
void KingClaw_EarlyUpdate();
void KingClaw_LateUpdate();
void KingClaw_Draw();
void KingClaw_Setup(void* subtype);
void KingClaw_StageLoad();
void KingClaw_GetAttributes();

#endif //!OBJ_KINGCLAW_H
