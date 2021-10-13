#ifndef OBJ_FORCEUNSTICK_H
#define OBJ_FORCEUNSTICK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
} ObjectForceUnstick;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 width;
    uint8 height;
    bool32 breakClimb;
    Hitbox hitbox;
} EntityForceUnstick;

// Object Struct
extern ObjectForceUnstick *ForceUnstick;

// Standard Entity Events
void ForceUnstick_Update(void);
void ForceUnstick_LateUpdate(void);
void ForceUnstick_StaticUpdate(void);
void ForceUnstick_Draw(void);
void ForceUnstick_Create(void* data);
void ForceUnstick_StageLoad(void);
void ForceUnstick_EditorDraw(void);
void ForceUnstick_EditorLoad(void);
void ForceUnstick_Serialize(void);

// Extra Entity Functions
void ForceUnstick_DrawSprites(void);


#endif //!OBJ_FORCEUNSTICK_H
