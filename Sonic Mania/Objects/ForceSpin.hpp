#ifndef OBJ_FORCESPIN_H
#define OBJ_FORCESPIN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectForceSpin : Object {
    ushort spriteIndex;
};

// Entity Class
struct EntityForceSpin : Entity {
    int size;
    int negAngle;
    EntityAnimationData data;
};

// Object Struct
extern ObjectForceSpin *ForceSpin;

// Standard Entity Events
void ForceSpin_Update();
void ForceSpin_LateUpdate();
void ForceSpin_StaticUpdate();
void ForceSpin_Draw();
void ForceSpin_Create(void* data);
void ForceSpin_StageLoad();
void ForceSpin_EditorDraw();
void ForceSpin_EditorLoad();
void ForceSpin_Serialize();

// Extra Entity Functions
void ForceSpin_DrawSprites();
void ForceSpin_SetPlayerState(void *plr);

#endif //!OBJ_FORCESPIN_H
