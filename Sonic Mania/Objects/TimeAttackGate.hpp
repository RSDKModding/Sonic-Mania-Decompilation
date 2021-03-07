#ifndef OBJ_TIMEATTACKGATE_H
#define OBJ_TIMEATTACKGATE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTimeAttackGate : Object {

};

// Entity Class
struct EntityTimeAttackGate : Entity {

};

// Object Struct
extern ObjectTimeAttackGate *TimeAttackGate;

// Standard Entity Events
void TimeAttackGate_Update();
void TimeAttackGate_LateUpdate();
void TimeAttackGate_StaticUpdate();
void TimeAttackGate_Draw();
void TimeAttackGate_Create(void* data);
void TimeAttackGate_StageLoad();
void TimeAttackGate_EditorDraw();
void TimeAttackGate_EditorLoad();
void TimeAttackGate_Serialize();

// Extra Entity Functions


#endif //!OBJ_TIMEATTACKGATE_H
