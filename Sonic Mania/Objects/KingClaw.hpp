#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectKingClaw : Object {

};

// Entity Class
struct EntityKingClaw : Entity {

};

// Object Struct
extern ObjectKingClaw *KingClaw;

// Standard Entity Events
void KingClaw_Update();
void KingClaw_LateUpdate();
void KingClaw_StaticUpdate();
void KingClaw_Draw();
void KingClaw_Create(void* data);
void KingClaw_StageLoad();
void KingClaw_EditorDraw();
void KingClaw_EditorLoad();
void KingClaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_KINGCLAW_H
