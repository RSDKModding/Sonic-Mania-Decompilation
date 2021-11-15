#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

#include "SonicMania.h"

typedef enum { LRZCONVDROP_TRIGGER_BUTTON, LRZCONVDROP_TRIGGER_PLAYER } LRZConvDropperTriggerModes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectLRZConvDropper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 triggerMode;
    int32 interval;
    int32 intervalOffset;
    Vector2 detectOffset;
    Vector2 detectSize;
    uint8 seqCount;
    bool32 seqLoop;
    uint8 activePlayers;
    uint8 seqPos;
    int32 timerStore;
    int32 field_84;
    Hitbox hitbox;
    Animator animator;
} EntityLRZConvDropper;

// Object Struct
extern ObjectLRZConvDropper *LRZConvDropper;

// Standard Entity Events
void LRZConvDropper_Update(void);
void LRZConvDropper_LateUpdate(void);
void LRZConvDropper_StaticUpdate(void);
void LRZConvDropper_Draw(void);
void LRZConvDropper_Create(void* data);
void LRZConvDropper_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZConvDropper_EditorDraw(void);
void LRZConvDropper_EditorLoad(void);
#endif
void LRZConvDropper_Serialize(void);

// Extra Entity Functions
void LRZConvDropper_SetupDropperChildren(void);
void LRZConvDropper_HandleButtonDrop(EntityLRZConvDropper *entity);

#endif //!OBJ_LRZCONVDROPPER_H
