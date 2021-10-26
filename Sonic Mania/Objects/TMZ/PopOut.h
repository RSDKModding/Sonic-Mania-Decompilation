#ifndef OBJ_POPOUT_H
#define OBJ_POPOUT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 hasButton;
    uint16 aniFrames;
} ObjectPopOut;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 orientation;
    uint8 tag;
    bool32 manualTrigger;
    int32 delay;
    Vector2 field_64;
    int32 field_6C;
    int32 childType;
    bool32 flag;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    Hitbox hitbox2;
    Hitbox hitbox;
    Animator animator;
    Animator animator2;
} EntityPopOut;

// Object Struct
extern ObjectPopOut *PopOut;

// Standard Entity Events
void PopOut_Update(void);
void PopOut_LateUpdate(void);
void PopOut_StaticUpdate(void);
void PopOut_Draw(void);
void PopOut_Create(void* data);
void PopOut_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PopOut_EditorDraw(void);
void PopOut_EditorLoad(void);
#endif
void PopOut_Serialize(void);

// Extra Entity Functions
void PopOut_Unknown(void);

#endif //!OBJ_POPOUT_H
