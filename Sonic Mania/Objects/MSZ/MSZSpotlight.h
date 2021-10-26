#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectMSZSpotlight;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 color;
    int32 timer;
    int32 moveFlag;
    Vector2 startPos;
    Animator animator;
} EntityMSZSpotlight;

// Object Struct
extern ObjectMSZSpotlight *MSZSpotlight;

// Standard Entity Events
void MSZSpotlight_Update(void);
void MSZSpotlight_LateUpdate(void);
void MSZSpotlight_StaticUpdate(void);
void MSZSpotlight_Draw(void);
void MSZSpotlight_Create(void* data);
void MSZSpotlight_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void);
void MSZSpotlight_EditorLoad(void);
#endif
void MSZSpotlight_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSZSPOTLIGHT_H
