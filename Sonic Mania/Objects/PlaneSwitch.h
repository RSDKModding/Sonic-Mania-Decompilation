#ifndef OBJ_PLANESWITCH_H
#define OBJ_PLANESWITCH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectPlaneSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int flags;
    int size;
    bool32 onPath;
    int negAngle;
    EntityAnimationData data;
} EntityPlaneSwitch;

// Object Struct
extern ObjectPlaneSwitch *PlaneSwitch;

// Standard Entity Events
void PlaneSwitch_Update();
void PlaneSwitch_LateUpdate();
void PlaneSwitch_StaticUpdate();
void PlaneSwitch_Draw();
void PlaneSwitch_Create(void* data);
void PlaneSwitch_StageLoad();
void PlaneSwitch_EditorDraw();
void PlaneSwitch_EditorLoad();
void PlaneSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLANESWITCH_H
