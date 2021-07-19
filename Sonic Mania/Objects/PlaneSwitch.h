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
    Animator animator;
} EntityPlaneSwitch;

// Object Struct
extern ObjectPlaneSwitch *PlaneSwitch;

// Standard Entity Events
void PlaneSwitch_Update(void);
void PlaneSwitch_LateUpdate(void);
void PlaneSwitch_StaticUpdate(void);
void PlaneSwitch_Draw(void);
void PlaneSwitch_Create(void* data);
void PlaneSwitch_StageLoad(void);
void PlaneSwitch_EditorDraw(void);
void PlaneSwitch_EditorLoad(void);
void PlaneSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PLANESWITCH_H
